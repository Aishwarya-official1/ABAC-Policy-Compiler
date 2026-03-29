# ABAC Policy Compiler

## Description
This project implements a compiler for Attribute-Based Access Control (ABAC) policies. It performs lexical, syntax, and semantic analysis, generates an Intermediate Representation (IR), and compiles policies into structured enforcement formats.

## Features
- Lexical Analysis
- Syntax Parsing
- Semantic Validation
- IR Generation
- Policy Compilation (Rule-based, Table, JSON)
- Optimization and Security Checks (redundancy & conflict detection)

## Current Status
Implemented up to policy compilation and basic optimization:
- IR → Compiled outputs (rules, table, JSON)
- Redundant rule removal
- Conflict detection with security report generation

## Future Work
- Integration with authentication system (user login and attribute retrieval)
- Real-time policy enforcement engine
- Database/Identity Provider (IdP) integration for user attributes
- Advanced optimization techniques for large policy sets
- Web-based interface for policy management

## Tech Stack
C++

## How to Run
g++ *.cpp -o compiler
./compiler

## Output Files
- compiled_policy_rules.txt
- compiled_policy_table.txt
- compiled_policy.json
- security_analysis_report.txt
