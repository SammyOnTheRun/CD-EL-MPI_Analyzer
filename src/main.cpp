#include "MPIAnalyzer.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"

static llvm::cl::OptionCategory MPICategory("MPI Analyzer options");

int main(int argc, const char **argv) {
    // Use CommonOptionsParser::create to handle the protected constructor
    auto ExpectedParser = clang::tooling::CommonOptionsParser::create(argc, argv, MPICategory);
    if (!ExpectedParser) {
        llvm::errs() << "Failed to parse command-line options: "
                     << llvm::toString(ExpectedParser.takeError()) << "\n";
        return 1;
    }
    clang::tooling::CommonOptionsParser &OptionsParser = *ExpectedParser;
    clang::tooling::ClangTool Tool(OptionsParser.getCompilations(),
                                   OptionsParser.getSourcePathList());

    return Tool.run(clang::tooling::newFrontendActionFactory<MPIToolAction>().get());
}

