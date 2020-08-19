/*
 * Copyright (c) University of Illinois at Urbana-Champaign and University of Luxembourg 2020.
 * Created by Darko Marinov et al., marinov@illinois.edu, University of Illinois at Urbana-Champaign, 2018. See LICENCE.SRCIROR.
 * Modified by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, University of Luxembourg - SnT, 2020.
 *
 */
 
#include <fstream>
#include <set>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <vector>

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Lex/Lexer.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Rewrite/Frontend/Rewriters.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Refactoring.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Tooling/Core/Replacement.h"
#include "llvm/Support/raw_ostream.h"


using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::driver;
using namespace clang::tooling;

static llvm::cl::OptionCategory OptimuteMutatorCategory("Optimute Mutator");
static llvm::cl::opt<std::string> CoverageInfo("coverage_info", llvm::cl::desc("Specify the lines covered by the test suite and should mutate"), llvm::cl::value_desc("string"));
static std::set<int> CovSet;
static bool MutateAll = false;

std::map<int, std::string> mapFunctions;
std::map<Replacement, int> mapReplacements;


class StmntHandler : public MatchFinder::MatchCallback {

public:
	StmntHandler(Replacements* Replace, std::string Binder, CompilerInstance* TheCompInst) : Replace(Replace), CI(TheCompInst) {
		this->Binder = Binder;
	}

	virtual void run(const MatchFinder::MatchResult &Result) {

		if (const Stmt* Stmnt = Result.Nodes.getNodeAs<clang::Stmt>(Binder) ) {

			SourceLocation stmtLoc = Stmnt->getLocStart();
			int lineNumber;
			SourceLocation stmtLocStart, stmtLocEnd;

			if (Result.SourceManager->isMacroBodyExpansion(stmtLoc)){
				lineNumber = Result.SourceManager->getExpansionLineNumber(stmtLoc);
				stmtLocStart = Result.SourceManager->getExpansionLoc(Stmnt->getLocStart());
				stmtLocEnd = Result.SourceManager->getExpansionLoc(Stmnt->getLocEnd());

			} else{
				lineNumber = Result.SourceManager->getSpellingLineNumber(stmtLoc);
				stmtLocStart = Stmnt->getLocStart();
				stmtLocEnd = Stmnt->getLocEnd();
			}

			//printf("Found the line %i and it is covered\n", lineNumber);

			if (!MutateAll && CovSet.find(lineNumber) == CovSet.end()) {  // Line is not in the set, and it's not MutateAll
			// printf("The match we found: %i is not in the set of covered lines\n", lineNumber);
				return;
			}
			bool invalid;
			CharSourceRange statementRange = CharSourceRange::getTokenRange(stmtLocStart, stmtLocEnd);
			StringRef str = Lexer::getSourceText(statementRange, *(Result.SourceManager), CI->getLangOpts(), &invalid);

			std::string MutatedString = "";

			Replacement Rep(*(Result.SourceManager), stmtLocStart, str.str().size(), MutatedString);
			Replace->insert(Rep);
			mapReplacements[Rep] = 1;
		}
	}

private:
	Replacements *Replace;
	CompilerInstance *CI;
	std::string Binder;
};

class IfCondHandler : public MatchFinder::MatchCallback {

public:
	IfCondHandler(Replacements* Replace, std::string Binder, CompilerInstance* TheCompInst) : Replace(Replace), CI(TheCompInst) {
		this->Binder = Binder;
	}

	virtual void run(const MatchFinder::MatchResult &Result) {
		if (const IfStmt *CondStmt = Result.Nodes.getNodeAs<clang::IfStmt>(Binder)) {
			int lineNumber = Result.SourceManager->getSpellingLineNumber(CondStmt->getIfLoc());
			if (!MutateAll && CovSet.find(lineNumber) == CovSet.end()) {  // Line is not in the set, and it's not MutateAll
				// printf("The match we found: %i is not in the set of covered lines\n", lineNumber);
				return;
			}
			// printf("Found the line %i and it is covered\n", lineNumber);
			const Expr *Condition = CondStmt->getCond();
			bool invalid;

			CharSourceRange conditionRange = CharSourceRange::getTokenRange(Condition->getLocStart(), Condition->getLocEnd());
			StringRef str = Lexer::getSourceText(conditionRange, *(Result.SourceManager), CI->getLangOpts(), &invalid);
			std::string MutatedString = "!(" + str.str() + ")";
			Replacement Rep(*(Result.SourceManager), Condition->getLocStart(), str.str().size(), MutatedString);
			Replace->insert(Rep);
			mapReplacements[Rep] = 7;
		}
	}

private:
	Replacements *Replace;
	CompilerInstance *CI;
	std::string Binder;
};

class WhileCondHandler : public MatchFinder::MatchCallback {

public:
	WhileCondHandler(Replacements* Replace, std::string Binder, CompilerInstance* TheCompInst) : Replace(Replace), CI(TheCompInst) {
		this->Binder = Binder;
	}

	virtual void run(const MatchFinder::MatchResult &Result) {
		if (const WhileStmt *CondStmt = Result.Nodes.getNodeAs<clang::WhileStmt>(Binder)) {

			SourceLocation whileLoc = CondStmt->getCond()->getLocStart();

			int lineNumber;
			SourceLocation whileLocStart, whileLocEnd;

			if (Result.SourceManager->isMacroBodyExpansion(whileLoc)){
				lineNumber = Result.SourceManager->getExpansionLineNumber(whileLoc);
				whileLocStart = Result.SourceManager->getExpansionLoc(CondStmt->getCond()->getLocStart());
				whileLocEnd = Result.SourceManager->getExpansionLoc(CondStmt->getCond()->getLocEnd());
			} else {
				lineNumber = Result.SourceManager->getSpellingLineNumber(whileLoc);

				whileLocStart = CondStmt->getCond()->getLocStart();
				whileLocEnd = CondStmt->getCond()->getLocEnd();
			}

			if (!MutateAll && CovSet.find(lineNumber) == CovSet.end()) {  // Line is not in the set, and it's not MutateAll
				// printf("The match we found: %i is not in the set of covered lines\n", lineNumber);
				return;
			}
			//printf("Found the line %i and it is covered\n", lineNumber);
			const Expr *Condition = CondStmt->getCond();
			bool invalid;

			CharSourceRange conditionRange = CharSourceRange::getTokenRange(whileLocStart, whileLocEnd);  // Getting char size of condition
			StringRef str = Lexer::getSourceText(conditionRange, *(Result.SourceManager), CI->getLangOpts(), &invalid);
			std::string MutatedString = "!(" + str.str() + ")";
			Replacement Rep(*(Result.SourceManager), whileLocStart, str.str().size(), MutatedString);
			Replace->insert(Rep);
			mapReplacements[Rep] = 8;
		}
	}

private:
	Replacements *Replace;
	CompilerInstance *CI;
	std::string Binder;
};


class LitConstHandler: public MatchFinder::MatchCallback {

public:
	LitConstHandler(Replacements *Replace, std::string Binder,
			CompilerInstance *TheCompInst) :
			Replace(Replace), CI(TheCompInst) {
		this->Binder = Binder;
	}

	virtual void run(const MatchFinder::MatchResult &Result) {

		if (const IntegerLiteral *IntLiteral = Result.Nodes.getNodeAs<clang::IntegerLiteral>(Binder)) {
			SourceLocation loc = IntLiteral->getLocation();
			SourceLocation spellingLoc = Result.SourceManager->getSpellingLoc(loc);
			std::string fileNameResult = Result.SourceManager->getFilename(spellingLoc);

			if (fileNameResult.find("bool") == std::string::npos) {
				return;
			}

			int lineNumber = Result.SourceManager->getExpansionLineNumber(loc);

			SourceLocation expansionLoc = Result.SourceManager->getExpansionLoc(loc);

			if (!MutateAll && CovSet.find(lineNumber) == CovSet.end()) { // Line is not in the set, and it's not MutateAll
//				printf("The match we found: %i is not in the set of covered lines\n", lineNumber);
				return;
			}
//			printf("Found the line %i and it is covered\n", lineNumber);

			std::string ValueStr = IntLiteral->getValue().toString(10, true);
			char *endptr;
			long long Value = std::strtoll(ValueStr.c_str(), &endptr, 10);

			if (Value == 1) {
				Replacement Rep(*(Result.SourceManager), expansionLoc, 4, "false");
				Replace->insert(Rep);
				mapReplacements[Rep] = 4;
			} else if (Value == 0) {
				Replacement Rep(*(Result.SourceManager), expansionLoc, 5, "true");
				Replace->insert(Rep);
				mapReplacements[Rep] = 5;
			}
		} else if (const FloatingLiteral *FloatLiteral = Result.Nodes.getNodeAs<clang::FloatingLiteral>(Binder)) {

			int lineNumber = Result.SourceManager->getSpellingLineNumber(FloatLiteral->getLocation());
			if (!MutateAll && CovSet.find(lineNumber) == CovSet.end()) { // Line is not in the set, and it's not MutateAll
					// printf("The match we found: %i is not in the set of covered lines\n", lineNumber);
				return;
			}
			//	printf("Found the line %i and it is covered\n", lineNumber);
			double Value = FloatLiteral->getValueAsApproximateDouble();

			bool invalid;
			CharSourceRange range = CharSourceRange::getTokenRange(FloatLiteral->getLocStart(), FloatLiteral->getLocEnd());
			StringRef str = Lexer::getSourceText(range, *(Result.SourceManager), CI->getLangOpts(), &invalid);

			int Size = str.size();

			if (Value == 0) {
				Replacement Rep(*(Result.SourceManager), FloatLiteral->getLocStart(), Size, "(-1.0)");
				Replace->insert(Rep);
				mapReplacements[Rep] = 1;
			} else {
				Replacement Rep2(*(Result.SourceManager), FloatLiteral->getLocStart(), Size, "-(" + str.str() + ")");
				Replacement Rep3(*(Result.SourceManager), FloatLiteral->getLocStart(), Size, "0.0");
				Replace->insert(Rep2);
				Replace->insert(Rep3);
				mapReplacements[Rep2] = 2;
				mapReplacements[Rep3] = 3;
			}
		}
	}

private:
	Replacements *Replace;
	CompilerInstance *CI;
	std::string Binder;
};

class ConstHandler : public MatchFinder::MatchCallback {

public:
	ConstHandler(Replacements* Replace, std::string Binder) : Replace(Replace) {
		this->Binder = Binder;
	}

	virtual void run(const MatchFinder::MatchResult &Result) {

		if (const IntegerLiteral *IntLiteral = Result.Nodes.getNodeAs<clang::IntegerLiteral>(Binder)) {
			int lineNumber = Result.SourceManager->getSpellingLineNumber(IntLiteral->getLocation());
			if (!MutateAll && CovSet.find(lineNumber) == CovSet.end()) {  // Line is not in the set, and it's not MutateAll
				// printf("The match we found: %i is not in the set of covered lines\n", lineNumber);
				return;
			}
			// printf("Found the line %i and it is covered\n", lineNumber);

			std::string ValueStr = IntLiteral->getValue().toString(10, true);
			char* endptr;
			long long Value = std::strtoll(ValueStr.c_str(), &endptr, 10);
			int Size = ValueStr.size();

			if (Value == 1) {
				Replacement Rep1(*(Result.SourceManager), IntLiteral->getLocStart(), Size, "(-1)");
				Replacement Rep2(*(Result.SourceManager), IntLiteral->getLocStart(), Size, "0");
				Replacement Rep3(*(Result.SourceManager), IntLiteral->getLocStart(), Size, "2");
				Replace->insert(Rep1);
				Replace->insert(Rep2);
				Replace->insert(Rep3);
				mapReplacements[Rep1] = 2;
				mapReplacements[Rep2] = 3;
				mapReplacements[Rep3] = 4;

			}
			else if (Value == -1) {
				Replacement Rep1(*(Result.SourceManager), IntLiteral->getLocStart(), Size, "1");
				Replacement Rep2(*(Result.SourceManager), IntLiteral->getLocStart(), Size, "0");
				Replacement Rep3(*(Result.SourceManager), IntLiteral->getLocStart(), Size, "(-2)");
				Replace->insert(Rep1);
				Replace->insert(Rep2);
				Replace->insert(Rep3);
				mapReplacements[Rep1] = 1;
				mapReplacements[Rep2] = 3;
				mapReplacements[Rep3] = 5;
			}
			else if (Value == 0) {
				Replacement Rep1(*(Result.SourceManager), IntLiteral->getLocStart(), Size, "1");
				Replacement Rep2(*(Result.SourceManager), IntLiteral->getLocStart(), Size, "(-1)");
				Replace->insert(Rep1);
				Replace->insert(Rep2);
				mapReplacements[Rep1] = 1;
				mapReplacements[Rep2] = 2;
			}
			else {
				Replacement Rep1(*(Result.SourceManager), IntLiteral->getLocStart(), Size, "1");
				Replacement Rep2(*(Result.SourceManager), IntLiteral->getLocStart(), Size, "(-1)");
				Replacement Rep3(*(Result.SourceManager), IntLiteral->getLocStart(), Size, "0");
				Replacement Rep4(*(Result.SourceManager), IntLiteral->getLocStart(), Size, "("  + ValueStr + " + 1)");
				Replacement Rep5(*(Result.SourceManager), IntLiteral->getLocStart(), Size, "("  + ValueStr + " - 1)");
				Replacement Rep6(*(Result.SourceManager), IntLiteral->getLocStart(), Size, "-("  + ValueStr + ")");
				Replace->insert(Rep1);
				Replace->insert(Rep2);
				Replace->insert(Rep3);
				Replace->insert(Rep4);
				Replace->insert(Rep5);
				Replace->insert(Rep6);
				mapReplacements[Rep1] = 1;
				mapReplacements[Rep2] = 2;
				mapReplacements[Rep3] = 3;
				mapReplacements[Rep4] = 4;
				mapReplacements[Rep5] = 5;
				mapReplacements[Rep6] = 6;
			}

		}
	}

private:
	Replacements *Replace;
	std::string Binder;
};


class BinaryOpDelHandler : public MatchFinder::MatchCallback {

public:
	BinaryOpDelHandler(Replacements* Replace, std::string OpName, CompilerInstance* TheCompInst) : Replace(Replace), CI(TheCompInst) {
		this->OpName = OpName;
	}

	virtual void run(const MatchFinder::MatchResult &Result) {

		if (const BinaryOperator *BinOp = Result.Nodes.getNodeAs<clang::BinaryOperator>(OpName)) {
			int lineNumber = Result.SourceManager->getSpellingLineNumber(BinOp->getOperatorLoc());
			if (!MutateAll && CovSet.find(lineNumber) == CovSet.end()) {  // Line is not in the set, and it's not MutateAll
				// printf("The match we found: %i is not in the set of covered lines\n", lineNumber);
				return;
			}
			// printf("Found the line %i and it is covered\n", lineNumber);

			Expr * rhs = BinOp->getRHS();
			Expr * lhs = BinOp->getLHS();

			bool invalid;

			CharSourceRange sourceLeftRange = CharSourceRange::getTokenRange(lhs->getLocStart(), BinOp->getOperatorLoc());
			StringRef leftString = Lexer::getSourceText(sourceLeftRange, *(Result.SourceManager), CI->getLangOpts(), &invalid);

			CharSourceRange sourceRightRange = CharSourceRange::getTokenRange(BinOp->getOperatorLoc(), rhs->getLocEnd());
			StringRef rightString = Lexer::getSourceText(sourceRightRange, *(Result.SourceManager), CI->getLangOpts(), &invalid);

			std::string MutatedString = "";

			// left hand side
			int totalLeftSize = leftString.str().size();
			Replacement RepL(*(Result.SourceManager), lhs->getLocStart(), totalLeftSize, MutatedString);
			Replace->insert(RepL);
			mapReplacements[RepL] = 1;

			// right hand side
			int totalRightSize = rightString.str().size();
			Replacement RepR(*(Result.SourceManager), BinOp->getOperatorLoc(), totalRightSize, MutatedString);
			Replace->insert(RepR);
			mapReplacements[RepR] = 2;
		}
	}

private:
	Replacements *Replace;
	CompilerInstance *CI;
	std::string OpName;
};


class BinaryOpHandler : public MatchFinder::MatchCallback {

public:
	BinaryOpHandler(Replacements* Replace, std::string OpName, std::string Op, std::string OpType) : Replace(Replace) {
		this->OpName = OpName;
		this->Op = Op;
		this->OpType = OpType;
		if (OpType == "arith") {

			CurrOps = &ArithmeticOperators;
		}
		else if (OpType == "rel") {
			CurrOps = &RelationalOperators;
		}
		else if (OpType == "logical") {
			CurrOps = &LogicalOperators;
		}
		else if (OpType == "bitwise") {
			CurrOps = &BitwiseOperators;
		}
		else if (OpType == "arithAssign") {
			CurrOps = &ArithAssignOperators;
		}
		else if (OpType == "bitwiseAssign") {
			CurrOps = &BitwiseAssignOperators;
		}

	}

	virtual void run(const MatchFinder::MatchResult &Result) {

		if (const BinaryOperator *BinOp = Result.Nodes.getNodeAs<clang::BinaryOperator>(OpName)) {
			int lineNumber = Result.SourceManager->getSpellingLineNumber(BinOp->getOperatorLoc());
			if (!MutateAll && CovSet.find(lineNumber) == CovSet.end()) {  // Line is not in the set, and it's not MutateAll
				// printf("The match we found: %i is not in the set of covered lines\n", lineNumber);
				return;
			}
			// printf("Found the line %i and it is covered\n", lineNumber);

			int Size = Op.size();
			for (std::string MutationOp : *CurrOps) {
				if (MutationOp == Op) {
					continue;
				}
				Replacement Rep(*(Result.SourceManager), BinOp->getOperatorLoc(), Size, MutationOp);
				Replace->insert(Rep);
				mapReplacements[Rep] = localMap[MutationOp];
			}
		}
	}


private:
	Replacements *Replace;
	std::string OpName;
	std::string Op;
	std::string OpType;

	std::vector<std::string>* CurrOps;
	std::vector<std::string> ArithmeticOperators = {"+", "-", "*", "/", "%"};
	std::vector<std::string> RelationalOperators = {"<", "<=", ">", ">=", "==", "!="};
	std::vector<std::string> LogicalOperators = {"&&", "||"};
	std::vector<std::string> BitwiseOperators = {"&", "|", "^"};
	std::vector<std::string> ArithAssignOperators = {"+=", "-=", "*=", "/=", "%="};
	std::vector<std::string> BitwiseAssignOperators = {"&=", "|=", "^="};

	std::map<std::string, int> localMap = {
										{"+", 1}, {"-", 2}, {"*", 3}, {"/", 4}, {"%", 5},
										{"+=", 6}, {"-=", 7}, {"*=", 8}, {"/=", 9}, {"%=", 10},
										{"&&", 1}, {"||", 2},
										{"&=", 3}, {"|=", 4}, {"^=", 5},
										{"&", 6}, {"|", 7}, {"^", 8},
										{">", 1}, {">=", 2}, {"<", 3}, {"<=", 4}, {"==", 5}, {"!=", 6}
										};

};


class VarHandler: public MatchFinder::MatchCallback {

public:
	VarHandler(Replacements *Replace, std::string Binder, CompilerInstance *TheCompInst, std::string Op) :
			Replace(Replace), CI(TheCompInst) {
		this->Binder = Binder;
		this->Op = Op;
	}

	virtual void run(const MatchFinder::MatchResult &Result) {

		if (const DeclRefExpr *declRefExpr = Result.Nodes.getNodeAs<clang::DeclRefExpr>(Binder)) {

			int lineNumber = Result.SourceManager->getSpellingLineNumber(declRefExpr->getLocStart());
			if (!MutateAll && CovSet.find(lineNumber) == CovSet.end()) { // Line is not in the set, and it's not MutateAll
					// printf("The match we found: %i is not in the set of covered lines\n", lineNumber);
				return;
			}

			std::string Value = declRefExpr->getFoundDecl()->getNameAsString();

			if (Op == "ABS") {
				Replacement Rep(*(Result.SourceManager), declRefExpr->getLocStart(), Value.size(), "-(" + Value + ")");
				Replace->insert(Rep);
				mapReplacements[Rep] = 1;
			} else if (Op == "UOI") {
				Replacement Rep1(*(Result.SourceManager), declRefExpr->getLocStart(), Value.size(), "(--" + Value + ")");
				Replacement Rep2(*(Result.SourceManager), declRefExpr->getLocStart(), Value.size(), "(" + Value + "--)");
				Replacement Rep3(*(Result.SourceManager), declRefExpr->getLocStart(), Value.size(), "(++" + Value + ")");
				Replacement Rep4(*(Result.SourceManager), declRefExpr->getLocStart(), Value.size(), "(" + Value + "++)");
				Replace->insert(Rep1);
				Replace->insert(Rep2);
				Replace->insert(Rep3);
				Replace->insert(Rep4);

				mapReplacements[Rep1] = 1;
				mapReplacements[Rep2] = 2;
				mapReplacements[Rep3] = 3;
				mapReplacements[Rep4] = 4;
			}
		}
	}

private:
	Replacements *Replace;
	CompilerInstance *CI;
	std::string Binder;
	std::string Op;
	std::string varType;
};


class DeclFunctionHandler : public MatchFinder::MatchCallback {

public:
	DeclFunctionHandler( std::string Binder) {
		this->Binder = Binder;
	}

	virtual void run(const MatchFinder::MatchResult &Result) {

		if (const FunctionDecl* decl = Result.Nodes.getNodeAs<clang::FunctionDecl>(Binder) ) {

			int lineNumberStart = Result.SourceManager->getSpellingLineNumber(decl->getLocStart());
			int lineNumberEnd = Result.SourceManager->getSpellingLineNumber(decl->getLocEnd());

			int i;
			for (i = lineNumberStart; i <= lineNumberEnd; i++){
				mapFunctions[i] = decl->getNameInfo().getName().getAsString();
			}
		}
	}

private:
	std::string Binder;
};



bool applyReplacement(const Replacement &Replace, Rewriter &Rewrite) {
	bool Result = true;
	if (Replace.isApplicable()) {
		Result = Replace.apply(Rewrite);
	}
	else {
		Result = false;
	}

	return Result;
}


void Mutate(Replacements& repl, std::string NameSuffix, std::string tool, std::string ext, std::string srcDir, SourceManager& SourceMgr, CompilerInstance& TheCompInst, FileManager& FileMgr){
	int x = 0;

	int previousLineNumber = 0;
	int idCount= 1;

	for (auto &r : repl) {
		x++;
		std::string pName = r.getFilePath().str();

		std::string fName = tool + ext;
		if (pName.length() >= fName.length()) {
			if (pName.compare(pName.length() - fName.length(), fName.length(), fName) == 0) {
				Rewriter TheRewriter;
				TheRewriter.setSourceMgr(SourceMgr, TheCompInst.getLangOpts());

				// Set the main file handled by the source manager to the input file.
				const FileEntry *FileIn = FileMgr.getFile(srcDir + "/" + fName);
				SourceMgr.setMainFileID(
						SourceMgr.createFileID(FileIn, SourceLocation(), SrcMgr::C_User));
				TheCompInst.getDiagnosticClient().BeginSourceFile(
						TheCompInst.getLangOpts(), &TheCompInst.getPreprocessor());

				applyReplacement(r, TheRewriter);
				const RewriteBuffer *RewriteBuf = TheRewriter.getRewriteBufferFor(SourceMgr.getMainFileID());

				SourceLocation startLoc = SourceMgr.getLocForStartOfFile(SourceMgr.getMainFileID());
				SourceLocation mutloc = startLoc.getLocWithOffset(r.getOffset());
				int lineNumber = SourceMgr.getSpellingLineNumber(mutloc);

				std::string id;
				if (lineNumber == previousLineNumber){
					id = std::to_string(mapReplacements[r]) + "_" + std::to_string(++idCount);
				} else {
					id = std::to_string(mapReplacements[r]) + "_1";
					idCount = 1;
				}
				previousLineNumber = lineNumber;

				std::string function = mapFunctions[lineNumber];

				std::ofstream out_file;
				std::string outFileName = srcDir + "/" + tool + ".mut." + std::to_string(lineNumber) + "." +  id + "." + NameSuffix + "." + function + ext;
				if (access(outFileName.c_str(), F_OK) == -1) {
					out_file.open(outFileName);
					out_file << std::string(RewriteBuf->begin(), RewriteBuf->end());
					out_file.close();

					printf("line: %i %s\n", lineNumber, (tool + ".mut." + std::to_string(lineNumber) + "." + id + "." +  NameSuffix + "." + function + ext).c_str());    // Outputting where mutants are
				}
				else {
					printf("ERROR IN GENERATING MUTANTS: we have a name overlap for %s\n", outFileName.c_str());
				}
			}
		}
	}
}


std::set<int> parseCoverageLines(std::string ListOfLines) {
	std::stringstream ss(ListOfLines);
	std::set<int> result;

	while (ss.good()) {
		std::string substr;
		getline(ss, substr, ',');
		// If the value is "all", then means want to mutate all, so MutateAll=true
		if (!substr.compare("all")) {
			MutateAll = true;
			return result;
		}
		result.insert(std::stoi(substr));
	}

	return result;
}


int main(int argc, const char **argv) {
	CommonOptionsParser op(argc, argv, OptimuteMutatorCategory);

	RefactoringTool AODTool(op.getCompilations(), op.getSourcePathList());
	RefactoringTool LODTool(op.getCompilations(), op.getSourcePathList());
	RefactoringTool RODTool(op.getCompilations(), op.getSourcePathList());
	RefactoringTool BODTool(op.getCompilations(), op.getSourcePathList());
	RefactoringTool SODTool(op.getCompilations(), op.getSourcePathList());

	RefactoringTool AORTool(op.getCompilations(), op.getSourcePathList());
	RefactoringTool RORTool(op.getCompilations(), op.getSourcePathList());
	RefactoringTool ICRTool(op.getCompilations(), op.getSourcePathList());
	RefactoringTool LCRTool(op.getCompilations(), op.getSourcePathList());
	RefactoringTool OCNGTool(op.getCompilations(), op.getSourcePathList());
	RefactoringTool SSDLTool(op.getCompilations(), op.getSourcePathList());
	RefactoringTool UOITool(op.getCompilations(), op.getSourcePathList());
	RefactoringTool ABSTool(op.getCompilations(), op.getSourcePathList());

	RefactoringTool LVRTool(op.getCompilations(), op.getSourcePathList());

	RefactoringTool DeclTool(op.getCompilations(), op.getSourcePathList());


	CompilerInstance TheCompInst;
	TheCompInst.createDiagnostics();

	LangOptions &lo = TheCompInst.getLangOpts();
	lo.CPlusPlus = 1; // Works on C++ code

	// Initialize target info with the default triple for our platform.
	auto TO = std::make_shared<TargetOptions>();
	TO->Triple = llvm::sys::getDefaultTargetTriple();
	TargetInfo *TI =  TargetInfo::CreateTargetInfo(TheCompInst.getDiagnostics(), TO);
	TheCompInst.setTarget(TI);

	TheCompInst.createFileManager();
	FileManager &FileMgr = TheCompInst.getFileManager();
	TheCompInst.createSourceManager(FileMgr);
	SourceManager &SourceMgr = TheCompInst.getSourceManager();
	TheCompInst.createPreprocessor(TU_Module);
	TheCompInst.createASTContext();


	// Set up AST matcher callbacks.

	BinaryOpDelHandler HandlerForDelAddOp(&AODTool.getReplacements(), "addDelOp", &TheCompInst);
	BinaryOpDelHandler HandlerForDelSubOp(&AODTool.getReplacements(), "subDelOp", &TheCompInst);
	BinaryOpDelHandler HandlerForDelMulOp(&AODTool.getReplacements(), "mulDelOp",  &TheCompInst);
	BinaryOpDelHandler HandlerForDelDivOp(&AODTool.getReplacements(), "divDelOp", &TheCompInst);
	BinaryOpDelHandler HandlerForDelModuloOp(&AODTool.getReplacements(), "moduloDelOp", &TheCompInst);

	BinaryOpDelHandler HandlerForDelAndOp(&LODTool.getReplacements(), "logicAndDelOp", &TheCompInst);
	BinaryOpDelHandler HandlerForDelOrOp(&LODTool.getReplacements(), "logicOrDelOp", &TheCompInst);

	BinaryOpDelHandler HandlerForDelLtOp(&RODTool.getReplacements(), "ltDelOp", &TheCompInst);
	BinaryOpDelHandler HandlerForDelLeOp(&RODTool.getReplacements(), "leDelOp", &TheCompInst);
	BinaryOpDelHandler HandlerForDelGtOp(&RODTool.getReplacements(), "gtDelOp", &TheCompInst);
	BinaryOpDelHandler HandlerForDelGeOp(&RODTool.getReplacements(), "geDelOp", &TheCompInst);
	BinaryOpDelHandler HandlerForDelEqOp(&RODTool.getReplacements(), "eqDelOp", &TheCompInst);
	BinaryOpDelHandler HandlerForDelNeqOp(&RODTool.getReplacements(), "neqDelOp", &TheCompInst);

	BinaryOpDelHandler HandlerForDelBitAndOp(&BODTool.getReplacements(), "bitwiseAndDelOp", &TheCompInst);
	BinaryOpDelHandler HandlerForDelBitOrOp(&BODTool.getReplacements(), "bitwiseOrDelOp", &TheCompInst);
	BinaryOpDelHandler HandlerForDelBitXorOp(&BODTool.getReplacements(), "bitwiseXorDelOp", &TheCompInst);

	BinaryOpDelHandler HandlerForDelLShiftOp(&SODTool.getReplacements(), "leftShiftDelOp", &TheCompInst);
	BinaryOpDelHandler HandlerForDelRShiftOp(&SODTool.getReplacements(), "rightShiftDelOp", &TheCompInst);

	BinaryOpHandler HandlerForAddOp(&AORTool.getReplacements(), "addOp", "+", "arith");
	BinaryOpHandler HandlerForSubOp(&AORTool.getReplacements(), "subOp", "-", "arith");
	BinaryOpHandler HandlerForMulOp(&AORTool.getReplacements(), "mulOp", "*", "arith");
	BinaryOpHandler HandlerForDivOp(&AORTool.getReplacements(), "divOp", "/", "arith");
	BinaryOpHandler HandlerForModuloOp(&AORTool.getReplacements(), "moduloOp", "%", "arith");

	BinaryOpHandler HandlerForLtOp(&RORTool.getReplacements(), "ltOp", "<", "rel");
	BinaryOpHandler HandlerForLeOp(&RORTool.getReplacements(), "leOp", "<=", "rel");
	BinaryOpHandler HandlerForGtOp(&RORTool.getReplacements(), "gtOp", ">", "rel");
	BinaryOpHandler HandlerForGeOp(&RORTool.getReplacements(), "geOp", ">=", "rel");
	BinaryOpHandler HandlerForEqOp(&RORTool.getReplacements(), "eqOp", "==", "rel");
	BinaryOpHandler HandlerForNeqOp(&RORTool.getReplacements(), "neqOp", "!=", "rel");

	BinaryOpHandler HandlerForAndOp(&LCRTool.getReplacements(), "logicAndOp", "&&", "logical");
	BinaryOpHandler HandlerForOrOp(&LCRTool.getReplacements(), "logicOrOp", "||", "logical");

	BinaryOpHandler HandlerForBitAndOp(&LCRTool.getReplacements(), "bitwiseAndOp", "&", "bitwise");
	BinaryOpHandler HandlerForBitOrOp(&LCRTool.getReplacements(), "bitwiseOrOp", "|", "bitwise");
	BinaryOpHandler HandlerForBitXorOp(&LCRTool.getReplacements(), "bitwiseXorOp", "^", "bitwise");

	BinaryOpHandler HandlerForAddAssignOp(&AORTool.getReplacements(), "addAssignOp", "+=", "arithAssign");
	BinaryOpHandler HandlerForSubAssignOp(&AORTool.getReplacements(), "subAssignOp", "-=", "arithAssign");
	BinaryOpHandler HandlerForMulAssignOp(&AORTool.getReplacements(), "mulAssignOp", "*=", "arithAssign");
	BinaryOpHandler HandlerForDivAssignOp(&AORTool.getReplacements(), "divAssignOp", "/=", "arithAssign");
	BinaryOpHandler HandlerForModuloAssignOp(&AORTool.getReplacements(), "moduloAssignOp", "%=", "arithAssign");


	BinaryOpHandler HandlerForAndAssignOp(&LCRTool.getReplacements(), "andAssignOp", "&=", "bitwiseAssign");
	BinaryOpHandler HandlerForOrAssignOp(&LCRTool.getReplacements(), "orAssignOp", "|=", "bitwiseAssign");
	BinaryOpHandler HandlerForXorAssignOp(&LCRTool.getReplacements(), "xorAssignOp", "^=", "bitwiseAssign");

	ConstHandler HandlerForConst(&ICRTool.getReplacements(), "intConst");

	LitConstHandler HandlerForFloatLitConst(&LVRTool.getReplacements(), "floatLitConst", &TheCompInst);
	LitConstHandler HandlerForBoolLitConst(&LVRTool.getReplacements(), "boolLitConst", &TheCompInst);

	IfCondHandler HandlerForIf(&OCNGTool.getReplacements(), "if", &TheCompInst);
	WhileCondHandler HandlerForWhile(&OCNGTool.getReplacements(), "while", &TheCompInst);

	StmntHandler HandlerForStmnt(&SSDLTool.getReplacements(), "stmt", &TheCompInst);

	DeclFunctionHandler HandlerForDeclFunction("functionNames");

	VarHandler HandlerForUOI(&UOITool.getReplacements(), "uoi", &TheCompInst, "UOI");
	VarHandler HandlerForABS(&ABSTool.getReplacements(), "abs", &TheCompInst, "ABS");

	MatchFinder AODFinder;
	AODFinder.addMatcher(binaryOperator(hasOperatorName("+")).bind("addDelOp"), &HandlerForDelAddOp);
	AODFinder.addMatcher(binaryOperator(hasOperatorName("-")).bind("subDelOp"), &HandlerForDelSubOp);
	AODFinder.addMatcher(binaryOperator(hasOperatorName("*")).bind("mulDelOp"), &HandlerForDelMulOp);
	AODFinder.addMatcher(binaryOperator(hasOperatorName("/")).bind("divDelOp"), &HandlerForDelDivOp);
	AODFinder.addMatcher(binaryOperator(hasOperatorName("%")).bind("moduloDelOp"), &HandlerForDelModuloOp);

	MatchFinder LODFinder;
	LODFinder.addMatcher(binaryOperator(hasOperatorName("&&")).bind("logicAndDelOp"), &HandlerForDelAndOp);
	LODFinder.addMatcher(binaryOperator(hasOperatorName("||")).bind("logicOrDelOp"), &HandlerForDelOrOp);

	MatchFinder RODFinder;
	RODFinder.addMatcher(binaryOperator(hasOperatorName("<")).bind("ltDelOp"), &HandlerForDelLtOp);
	RODFinder.addMatcher(binaryOperator(hasOperatorName("<=")).bind("leDelOp"), &HandlerForDelLeOp);
	RODFinder.addMatcher(binaryOperator(hasOperatorName(">")).bind("gtDelOp"), &HandlerForDelGtOp);
	RODFinder.addMatcher(binaryOperator(hasOperatorName(">=")).bind("geDelOp"), &HandlerForDelGeOp);
	RODFinder.addMatcher(binaryOperator(hasOperatorName("==")).bind("eqDelOp"), &HandlerForDelEqOp);
	RODFinder.addMatcher(binaryOperator(hasOperatorName("!=")).bind("neqDelOp"), &HandlerForDelNeqOp);

	MatchFinder BODFinder;
	BODFinder.addMatcher(binaryOperator(hasOperatorName("&")).bind("bitwiseAndDelOp"), &HandlerForDelBitAndOp);
	BODFinder.addMatcher(binaryOperator(hasOperatorName("|")).bind("bitwiseOrDelOp"), &HandlerForDelBitOrOp);
	BODFinder.addMatcher(binaryOperator(hasOperatorName("^")).bind("bitwiseXorDelOp"), &HandlerForDelBitXorOp);

	MatchFinder SODFinder;
	SODFinder.addMatcher(binaryOperator(hasOperatorName(">>")).bind("leftShiftDelOp"), &HandlerForDelLShiftOp);
	SODFinder.addMatcher(binaryOperator(hasOperatorName("<<")).bind("rightShiftDelOp"), &HandlerForDelRShiftOp);

	MatchFinder AORFinder;
	AORFinder.addMatcher(binaryOperator(hasOperatorName("+")).bind("addOp"), &HandlerForAddOp);
	AORFinder.addMatcher(binaryOperator(hasOperatorName("-")).bind("subOp"), &HandlerForSubOp);
	AORFinder.addMatcher(binaryOperator(hasOperatorName("*")).bind("mulOp"), &HandlerForMulOp);
	AORFinder.addMatcher(binaryOperator(hasOperatorName("/")).bind("divOp"), &HandlerForDivOp);
	AORFinder.addMatcher(binaryOperator(hasOperatorName("%")).bind("moduloOp"), &HandlerForModuloOp);

	AORFinder.addMatcher(binaryOperator(hasOperatorName("+=")).bind("addAssignOp"), &HandlerForAddAssignOp);
	AORFinder.addMatcher(binaryOperator(hasOperatorName("-=")).bind("subAssignOp"), &HandlerForSubAssignOp);
	AORFinder.addMatcher(binaryOperator(hasOperatorName("*=")).bind("mulAssignOp"), &HandlerForMulAssignOp);
	AORFinder.addMatcher(binaryOperator(hasOperatorName("/=")).bind("divAssignOp"), &HandlerForDivAssignOp);
	AORFinder.addMatcher(binaryOperator(hasOperatorName("%=")).bind("moduloAssignOp"), &HandlerForModuloAssignOp);


	MatchFinder RORFinder;
	RORFinder.addMatcher(binaryOperator(hasOperatorName("<")).bind("ltOp"), &HandlerForLtOp);
	RORFinder.addMatcher(binaryOperator(hasOperatorName("<=")).bind("leOp"), &HandlerForLeOp);
	RORFinder.addMatcher(binaryOperator(hasOperatorName(">")).bind("gtOp"), &HandlerForGtOp);
	RORFinder.addMatcher(binaryOperator(hasOperatorName(">=")).bind("geOp"), &HandlerForGeOp);
	RORFinder.addMatcher(binaryOperator(hasOperatorName("==")).bind("eqOp"), &HandlerForEqOp);
	RORFinder.addMatcher(binaryOperator(hasOperatorName("!=")).bind("neqOp"), &HandlerForNeqOp);

	MatchFinder LCRFinder;
	LCRFinder.addMatcher(binaryOperator(hasOperatorName("&&")).bind("logicAndOp"), &HandlerForAndOp);
	LCRFinder.addMatcher(binaryOperator(hasOperatorName("||")).bind("logicOrOp"), &HandlerForOrOp);

	LCRFinder.addMatcher(binaryOperator(hasOperatorName("&")).bind("bitwiseAndOp"), &HandlerForBitAndOp);
	LCRFinder.addMatcher(binaryOperator(hasOperatorName("|")).bind("bitwiseOrOp"), &HandlerForBitOrOp);
	LCRFinder.addMatcher(binaryOperator(hasOperatorName("^")).bind("bitwiseXorOp"), &HandlerForBitXorOp);

	LCRFinder.addMatcher(binaryOperator(hasOperatorName("&=")).bind("andAssignOp"), &HandlerForAndAssignOp);
	LCRFinder.addMatcher(binaryOperator(hasOperatorName("|=")).bind("orAssignOp"), &HandlerForOrAssignOp);
	LCRFinder.addMatcher(binaryOperator(hasOperatorName("^=")).bind("xorAssignOp"), &HandlerForXorAssignOp);

	MatchFinder ICRFinder;
	ICRFinder.addMatcher(integerLiteral().bind("intConst"), &HandlerForConst);

	MatchFinder LVRFinder;
	LVRFinder.addMatcher(floatLiteral().bind("floatLitConst"), &HandlerForFloatLitConst);
	LVRFinder.addMatcher(integerLiteral(anyOf(equals(1),equals(0))).bind("boolLitConst"), &HandlerForBoolLitConst);

	MatchFinder OCNGFinder;
	OCNGFinder.addMatcher(ifStmt().bind("if"), &HandlerForIf);
	OCNGFinder.addMatcher(whileStmt().bind("while"), &HandlerForWhile);

	MatchFinder SSDLFinder;
	SSDLFinder.addMatcher(stmt(hasParent(compoundStmt())).bind("stmt"), &HandlerForStmnt);

	MatchFinder UOIFinder;
	UOIFinder.addMatcher(declRefExpr(to(varDecl(hasType(isInteger()))), unless(hasParent(binaryOperator(hasOperatorName("="))))).bind("uoi"), &HandlerForUOI);

	MatchFinder ABSFinder;
	ABSFinder.addMatcher(declRefExpr(to(varDecl(hasType(builtinType()))), unless(hasParent(binaryOperator(hasOperatorName("="))))).bind("abs"), &HandlerForABS);


	std::string FileName = argv[1];   // Assumes only one source file on command line to mutate
	CovSet = parseCoverageLines(CoverageInfo);
	std::string ToolDotExt = FileName.substr(FileName.find_last_of("/\\") + 1);
	std::string::size_type const p(ToolDotExt.find_last_of('.'));
	std::string CurrTool = ToolDotExt.substr(0, p);
	std::string Ext = ToolDotExt.substr(p, ToolDotExt.length());
	std::string SrcDir = FileName.substr(0, FileName.find_last_of("/\\"));

	// populate the function names
	MatchFinder DeclFunctionFinder;
	DeclFunctionFinder.addMatcher(functionDecl().bind("functionNames"), &HandlerForDeclFunction);

	DeclTool.run(newFrontendActionFactory(&DeclFunctionFinder).get());

	int failed = 0;

	if (int Result = AORTool.run(newFrontendActionFactory(&AORFinder).get())) {
		failed = 1;
	}
	else {
		Mutate(AORTool.getReplacements(), "AOR", CurrTool, Ext, SrcDir, SourceMgr, TheCompInst, FileMgr);
	}
	//-----
	if (int Result = RORTool.run(newFrontendActionFactory(&RORFinder).get())) {
		failed = 1;
	}
	else {
		Mutate(RORTool.getReplacements(), "ROR", CurrTool, Ext, SrcDir, SourceMgr, TheCompInst, FileMgr);
	}
	//-----
	if (int Result = ICRTool.run(newFrontendActionFactory(&ICRFinder).get())) {
		failed = 1;
	}
	else {
		Mutate(ICRTool.getReplacements(), "ICR", CurrTool, Ext, SrcDir, SourceMgr, TheCompInst, FileMgr);
	}
	//-----
	if (int Result = LCRTool.run(newFrontendActionFactory(&LCRFinder).get())) {
		failed = 1;
	}
	else {
		Mutate(LCRTool.getReplacements(), "LCR", CurrTool, Ext, SrcDir, SourceMgr, TheCompInst, FileMgr);
	}
	//-----
	if (int Result = OCNGTool.run(newFrontendActionFactory(&OCNGFinder).get())) {
		failed = 1;
	}
	else {
		Mutate(OCNGTool.getReplacements(), "ROR", CurrTool, Ext, SrcDir, SourceMgr, TheCompInst, FileMgr);
	}
	// -----
	if (int Result = SSDLTool.run(newFrontendActionFactory(&SSDLFinder).get())) {
		failed = 1;
	}
	else {
		Mutate(SSDLTool.getReplacements(), "SDL", CurrTool, Ext, SrcDir, SourceMgr, TheCompInst, FileMgr);
	}
	// -----
	if (int Result = UOITool.run(newFrontendActionFactory(&UOIFinder).get())) {
		failed = 1;
	} else {
		Mutate(UOITool.getReplacements(), "UOI", CurrTool, Ext, SrcDir, SourceMgr, TheCompInst, FileMgr);
	}
	// -----
	if (int Result = ABSTool.run(newFrontendActionFactory(&ABSFinder).get())) {
		failed = 1;
	} else {
		Mutate(ABSTool.getReplacements(), "ABS", CurrTool, Ext, SrcDir, SourceMgr, TheCompInst, FileMgr);
	}
	// -----
	if (int Result = AODTool.run(newFrontendActionFactory(&AODFinder).get())) {
		failed = 1;
	} else {
		Mutate(AODTool.getReplacements(), "AOD", CurrTool, Ext, SrcDir, SourceMgr, TheCompInst, FileMgr);
	}
	// -----
	if (int Result = LODTool.run(newFrontendActionFactory(&LODFinder).get())) {
		failed = 1;
	} else {
		Mutate(LODTool.getReplacements(), "LOD", CurrTool, Ext, SrcDir, SourceMgr, TheCompInst, FileMgr);
	}
	// -----
	if (int Result = RODTool.run(newFrontendActionFactory(&RODFinder).get())) {
			failed = 1;
	} else {
		Mutate(RODTool.getReplacements(), "ROD", CurrTool, Ext, SrcDir, SourceMgr, TheCompInst, FileMgr);
	}
	// -----
	if (int Result = BODTool.run(newFrontendActionFactory(&BODFinder).get())) {
			failed = 1;
	} else {
		Mutate(BODTool.getReplacements(), "BOD", CurrTool, Ext, SrcDir, SourceMgr, TheCompInst, FileMgr);
	}
	// -----
	if (int Result = SODTool.run(newFrontendActionFactory(&SODFinder).get())) {
		failed = 1;
	} else {
		Mutate(SODTool.getReplacements(), "SOD", CurrTool, Ext, SrcDir, SourceMgr, TheCompInst, FileMgr);
	}
	// -----
	if (int Result = LVRTool.run(newFrontendActionFactory(&LVRFinder).get())) {
			failed = 1;
	} else {
		Mutate(LVRTool.getReplacements(), "LVR", CurrTool, Ext, SrcDir, SourceMgr, TheCompInst, FileMgr);
	}

	return failed;
}
