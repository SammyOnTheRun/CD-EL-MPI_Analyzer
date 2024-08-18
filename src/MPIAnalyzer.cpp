#include "MPIAnalyzer.h"

bool MPIVisitor::VisitFunctionDecl(clang::FunctionDecl *F) {
    if (F->hasBody()) {
        clang::Stmt *FuncBody = F->getBody();

        for (auto *S : FuncBody->children()) {
            if (auto *IfStmt = llvm::dyn_cast<clang::IfStmt>(S)) {
                clang::Expr *Cond = IfStmt->getCond();
                if (auto *BinOp = llvm::dyn_cast<clang::BinaryOperator>(Cond)) {
                    if (BinOp->getOpcode() == clang::BO_EQ) {
                        if (auto *DRE = llvm::dyn_cast<clang::DeclRefExpr>(BinOp->getLHS())) {
                            if (DRE->getNameInfo().getName().getAsString() == "rank") {
                                llvm::outs() << "==============================\n";
                                llvm::outs() << "Analysis of " << F->getNameInfo().getName().getAsString() << " Function\n";
                                llvm::outs() << "==============================\n";
                                llvm::outs() << "Pattern Detected: Non-collective Data ";
                                if (F->getNameInfo().getName().getAsString() == "gatherData") {
                                    llvm::outs() << "Gathering\n";
                                } else if (F->getNameInfo().getName().getAsString() == "distributeData") {
                                    llvm::outs() << "Distribution\n";
                                }
                                llvm::outs() << "- Issue: Data is being " <<
                                    (F->getNameInfo().getName().getAsString() == "gatherData" ? "gathered from all processes to the root process" :
                                                                                               "distributed from the root process to all processes")
                                    << " without using collectives.\n";
                                
                                // Get the location and print it correctly
                                const clang::SourceManager &SM = Context->getSourceManager();  // Use const reference
                                llvm::outs() << "- Location: " << F->getNameInfo().getName().getAsString() << " function, Line "
                                             << SM.getSpellingLineNumber(F->getBeginLoc()) << "\n";
                                
                                llvm::outs() << "Details:\n";
                                IfStmt->getThen()->printPretty(llvm::outs(), nullptr, Context->getPrintingPolicy());
                                llvm::outs() << "\n\n";
                            }
                        }
                    }
                }
            }
        }
    }
    return true;
}

