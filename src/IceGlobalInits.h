//===- subzero/src/IceGlobalInits.h - Global declarations -------*- C++ -*-===//
//
//                        The Subzero Code Generator
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the representation of function declarations,
// global variable declarations, and the corresponding variable
// initializers in Subzero. Global variable initializers are
// represented as a sequence of simple initializers.
//
//===----------------------------------------------------------------------===//

#ifndef SUBZERO_SRC_ICEGLOBALINITS_H
#define SUBZERO_SRC_ICEGLOBALINITS_H

#include "llvm/IR/CallingConv.h"
#include "llvm/IR/GlobalValue.h" // for GlobalValue::LinkageTypes

#include "IceDefs.h"
#include "IceTypes.h"

// TODO(kschimpf): Remove ourselves from using LLVM representation for calling
// conventions and linkage types.

namespace Ice {

/// Base class for global variable and function declarations.
class GlobalDeclaration {
  GlobalDeclaration() = delete;
  GlobalDeclaration(const GlobalDeclaration &) = delete;
  GlobalDeclaration &operator=(const GlobalDeclaration &) = delete;

public:
  /// Discriminator for LLVM-style RTTI.
  enum GlobalDeclarationKind {
    FunctionDeclarationKind,
    VariableDeclarationKind
  };
  GlobalDeclarationKind getKind() const { return Kind; }
  const IceString &getName() const { return Name; }
  void setName(const IceString &NewName) { Name = NewName; }
  bool hasName() const { return !Name.empty(); }
  bool isInternal() const {
    return Linkage == llvm::GlobalValue::InternalLinkage;
  }
  llvm::GlobalValue::LinkageTypes getLinkage() const { return Linkage; }
  bool isExternal() const {
    return Linkage == llvm::GlobalValue::ExternalLinkage;
  }
  void setLinkage(llvm::GlobalValue::LinkageTypes NewLinkage) {
    Linkage = NewLinkage;
  }
  virtual ~GlobalDeclaration() {}

  /// Prints out type of the global declaration.
  virtual void dumpType(Ostream &Stream) const = 0;

  /// Prints out the global declaration.
  virtual void dump(GlobalContext *Ctx, Ostream &Stream) const = 0;
  void dump(Ostream &Stream) const {
    if (!ALLOW_DUMP)
      return;
    GlobalContext *const Ctx = nullptr;
    dump(Ctx, Stream);
  }

  /// Returns true if when emitting names, we should suppress mangling.
  virtual bool getSuppressMangling() const = 0;

  // Mangles name for cross tests, unless external and not defined locally
  // (so that relocations accross pnacl-sz and pnacl-llc will work).
  virtual IceString mangleName(GlobalContext *Ctx) const {
    return getSuppressMangling() ? Name : Ctx->mangleName(Name);
  }

protected:
  GlobalDeclaration(GlobalDeclarationKind Kind,
                    llvm::GlobalValue::LinkageTypes Linkage)
      : Kind(Kind), Linkage(Linkage) {}

  const GlobalDeclarationKind Kind;
  IceString Name;
  llvm::GlobalValue::LinkageTypes Linkage;
};

// Models a function declaration. This includes the type signature of
// the function, its calling conventions, and its linkage.
class FunctionDeclaration : public GlobalDeclaration {
  FunctionDeclaration() = delete;
  FunctionDeclaration(const FunctionDeclaration &) = delete;
  FunctionDeclaration &operator=(const FunctionDeclaration &) = delete;

public:
  static FunctionDeclaration *create(const FuncSigType &Signature,
                                     llvm::CallingConv::ID CallingConv,
                                     llvm::GlobalValue::LinkageTypes Linkage,
                                     bool IsProto);
  ~FunctionDeclaration() final {}
  const FuncSigType &getSignature() const { return Signature; }
  llvm::CallingConv::ID getCallingConv() const { return CallingConv; }
  // isProto implies that there isn't a (local) definition for the function.
  bool isProto() const { return IsProto; }
  static bool classof(const GlobalDeclaration *Addr) {
    return Addr->getKind() == FunctionDeclarationKind;
  }
  void dumpType(Ostream &Stream) const final;
  void dump(GlobalContext *Ctx, Ostream &Stream) const final;
  bool getSuppressMangling() const final { return isExternal() && IsProto; }

private:
  const Ice::FuncSigType Signature;
  llvm::CallingConv::ID CallingConv;
  bool IsProto;

  FunctionDeclaration(const FuncSigType &Signature,
                      llvm::CallingConv::ID CallingConv,
                      llvm::GlobalValue::LinkageTypes Linkage, bool IsProto)
      : GlobalDeclaration(FunctionDeclarationKind, Linkage),
        Signature(Signature), CallingConv(CallingConv), IsProto(IsProto) {}
};

/// Models a global variable declaration, and its initializers.
class VariableDeclaration : public GlobalDeclaration {
  VariableDeclaration(const VariableDeclaration &) = delete;
  VariableDeclaration &operator=(const VariableDeclaration &) = delete;

public:
  /// Base class for a global variable initializer.
  class Initializer {
    Initializer(const Initializer &) = delete;
    Initializer &operator=(const Initializer &) = delete;

  public:
    /// Discriminator for LLVM-style RTTI.
    enum InitializerKind {
      DataInitializerKind,
      ZeroInitializerKind,
      RelocInitializerKind
    };
    InitializerKind getKind() const { return Kind; }
    virtual ~Initializer() {}
    virtual SizeT getNumBytes() const = 0;
    virtual void dump(GlobalContext *Ctx, Ostream &Stream) const = 0;
    void dump(Ostream &Stream) const {
      if (ALLOW_DUMP)
        dump(nullptr, Stream);
    }
    virtual void dumpType(Ostream &Stream) const;

  protected:
    explicit Initializer(InitializerKind Kind) : Kind(Kind) {}

  private:
    const InitializerKind Kind;
  };

  // Models the data in a data initializer.
  typedef std::vector<char> DataVecType;

  /// Defines a sequence of byte values as a data initializer.
  class DataInitializer : public Initializer {
    DataInitializer(const DataInitializer &) = delete;
    DataInitializer &operator=(const DataInitializer &) = delete;

  public:
    template <class IntContainer>
    DataInitializer(const IntContainer &Values)
        : Initializer(DataInitializerKind), Contents(Values.size()) {
      size_t i = 0;
      for (auto &V : Values) {
        Contents[i] = static_cast<int8_t>(V);
        ++i;
      }
    }
    DataInitializer(const char *Str, size_t StrLen)
        : Initializer(DataInitializerKind), Contents(StrLen) {
      for (size_t i = 0; i < StrLen; ++i)
        Contents[i] = Str[i];
    }
    ~DataInitializer() override {}
    const DataVecType &getContents() const { return Contents; }
    SizeT getNumBytes() const final { return Contents.size(); }
    void dump(GlobalContext *Ctx, Ostream &Stream) const final;
    static bool classof(const Initializer *D) {
      return D->getKind() == DataInitializerKind;
    }

  private:
    // The byte contents of the data initializer.
    DataVecType Contents;
  };

  /// Defines a sequence of bytes initialized to zero.
  class ZeroInitializer : public Initializer {
    ZeroInitializer(const ZeroInitializer &) = delete;
    ZeroInitializer &operator=(const ZeroInitializer &) = delete;

  public:
    explicit ZeroInitializer(SizeT Size)
        : Initializer(ZeroInitializerKind), Size(Size) {}
    ~ZeroInitializer() override {}
    SizeT getNumBytes() const final { return Size; }
    void dump(GlobalContext *Ctx, Ostream &Stream) const final;
    static bool classof(const Initializer *Z) {
      return Z->getKind() == ZeroInitializerKind;
    }

  private:
    // The number of bytes to be zero initialized.
    SizeT Size;
  };

  /// Defines the relocation value of another global declaration.
  class RelocInitializer : public Initializer {
    RelocInitializer(const RelocInitializer &) = delete;
    RelocInitializer &operator=(const RelocInitializer &) = delete;

  public:
    RelocInitializer(const GlobalDeclaration *Declaration, RelocOffsetT Offset)
        : Initializer(RelocInitializerKind), Declaration(Declaration),
          Offset(Offset) {}
    ~RelocInitializer() override {}
    RelocOffsetT getOffset() const { return Offset; }
    const GlobalDeclaration *getDeclaration() const { return Declaration; }
    SizeT getNumBytes() const final { return RelocAddrSize; }
    void dump(GlobalContext *Ctx, Ostream &Stream) const final;
    void dumpType(Ostream &Stream) const final;
    static bool classof(const Initializer *R) {
      return R->getKind() == RelocInitializerKind;
    }

  private:
    // The global declaration used in the relocation.
    const GlobalDeclaration *Declaration;
    // The offset to add to the relocation.
    const RelocOffsetT Offset;
  };

  /// Models the list of initializers.
  typedef std::vector<Initializer *> InitializerListType;

  static VariableDeclaration *create();
  ~VariableDeclaration() final;

  const InitializerListType &getInitializers() const { return Initializers; }
  bool getIsConstant() const { return IsConstant; }
  void setIsConstant(bool NewValue) { IsConstant = NewValue; }
  uint32_t getAlignment() const { return Alignment; }
  void setAlignment(uint32_t NewAlignment) { Alignment = NewAlignment; }
  bool hasInitializer() const { return !Initializers.empty(); }
  bool hasNonzeroInitializer() const {
    return !(Initializers.size() == 1 &&
             llvm::isa<ZeroInitializer>(Initializers[0]));
  }

  /// Returns the number of bytes for the initializer of the global
  /// address.
  SizeT getNumBytes() const {
    SizeT Count = 0;
    for (Initializer *Init : Initializers) {
      Count += Init->getNumBytes();
    }
    return Count;
  }

  /// Adds Initializer to the list of initializers. Takes ownership of
  /// the initializer.
  void addInitializer(Initializer *Initializer) {
    Initializers.push_back(Initializer);
  }

  /// Prints out type for initializer associated with the declaration
  /// to Stream.
  void dumpType(Ostream &Stream) const final;

  /// Prints out the definition of the global variable declaration
  /// (including initialization).
  void dump(GlobalContext *Ctx, Ostream &Stream) const final;

  static bool classof(const GlobalDeclaration *Addr) {
    return Addr->getKind() == VariableDeclarationKind;
  }

  bool getSuppressMangling() const final {
    if (ForceSuppressMangling)
      return true;
    return isExternal() && !hasInitializer();
  }

  void setSuppressMangling() {
    ForceSuppressMangling = true;
  }

private:
  // list of initializers for the declared variable.
  InitializerListType Initializers;
  // The alignment of the declared variable.
  uint32_t Alignment;
  // True if a declared (global) constant.
  bool IsConstant;
  // If set to true, force getSuppressMangling() to return true.
  bool ForceSuppressMangling;

  VariableDeclaration()
      : GlobalDeclaration(VariableDeclarationKind,
                          llvm::GlobalValue::InternalLinkage),
        Alignment(0), IsConstant(false), ForceSuppressMangling(false) {}
};

template <class StreamType>
inline StreamType &operator<<(StreamType &Stream,
                              const VariableDeclaration::Initializer &Init) {
  Init.dump(Stream);
  return Stream;
}

template <class StreamType>
inline StreamType &operator<<(StreamType &Stream,
                              const GlobalDeclaration &Addr) {
  Addr.dump(Stream);
  return Stream;
}

} // end of namespace Ice

#endif // SUBZERO_SRC_ICEGLOBALINITS_H
