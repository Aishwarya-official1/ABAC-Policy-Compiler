# ABAC Policy Compiler

## Description
This project implements a compiler for Attribute-Based Access Control (ABAC) policies. It processes policies through lexical, syntax, and semantic analysis, generates an Intermediate Representation (IR), and compiles them into structured enforcement formats.

## Features
- Lexical Analysis
- Syntax Parsing
- Semantic Validation
- Intermediate Representation (IR) Generation
- Policy Compilation:
  - Rule-based representation
  - Tabular representation
  - JSON format
- Optimization and Security Analysis:
  - Redundant rule detection
  - Conflict detection between policies
  - Security analysis report generation

## Current Status
The project is implemented up to policy compilation and basic optimization:
- IR → compiled outputs (rules, table, JSON)
- Detection of redundant and conflicting rules
- Generation of security analysis report

## Output Files
- compiled_policy_rules.txt → human-readable rules
- compiled_policy_table.txt → tabular format
- compiled_policy.json → machine-readable format
- security_analysis_report.txt → redundancy & conflict analysis

## Future Work
- Integration with authentication system (user → role/attributes mapping)
- Real-time policy enforcement engine
- Database / Identity Provider (IdP) integration
- Advanced optimization for large-scale policies
- User interface for policy management

## Tech Stack
C++

## How to Run
g++ *.cpp -o compiler
./compiler

## Example Policy
IF subject.role == "student" AND resource.type == "notes" AND action == "read" AND department == "CSE"  
THEN ALLOW

