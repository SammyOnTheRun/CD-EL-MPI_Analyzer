#ifndef MPI_ANALYZER_H
#define MPI_ANALYZER_H

#include <clang/AST/ASTConsumer.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/Support/raw_ostream.h>

namespace clang {
    class ASTContext;
    class FunctionDecl;
    class ASTConsumer;
    class CompilerInstance;
    class ASTFrontendAction;
    class Decl;
}

// Visitor class to analyze MPI-related function declarations
class MPIVisitor : public clang::RecursiveASTVisitor<MPIVisitor> {
public:
    explicit MPIVisitor(const clang::ASTContext *Context) : Context(Context) {}

    bool VisitFunctionDecl(clang::FunctionDecl *F);

private:
    const clang::ASTContext *Context;
};

// Consumer class to handle translation units and utilize the MPIVisitor
class MPIASTConsumer : public clang::ASTConsumer {
public:
    explicit MPIASTConsumer(clang::ASTContext *Context) : Visitor(Context) {}

    // Handle the translation unit, traversing all declarations
    void HandleTranslationUnit(clang::ASTContext &Context) override {
        Visitor.TraverseDecl(Context.getTranslationUnitDecl());
    }

private:
    MPIVisitor Visitor;
};

// Action class to create an AST consumer for the plugin
class MPIToolAction : public clang::ASTFrontendAction {
public:
    // Create the AST consumer for the provided file
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
        clang::CompilerInstance &CI, llvm::StringRef file) override {
        return std::make_unique<MPIASTConsumer>(&CI.getASTContext());
    }
};

#endif // MPI_ANALYZER_H

