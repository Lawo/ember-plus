// libformula.test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <formula\libformula.hpp>
#include <cstdio>
#include <iostream>

int _tmain(int argc, _TCHAR* argv[])
{
    //auto term = libformula::TermCompiler::compile("exp(1 * 1)");
    //auto const r = term.compute(1.0);
    //return 0;
    // 
    // 

    auto input = std::string();
    auto it = 0.0;
    auto errors = libformula::ErrorStack();
    do
    {
        input = "";
        std::cout << "Enter term: ";
        std::getline(std::cin, input);

        if (input.size() > 1)
        {
            std::cout << std::endl << "Enter $   :";
            std::cin >> it;
            std::cin.ignore();

            errors.clear();
            auto term = libformula::TermCompiler::compile(input, &errors);
            auto const result = term.compute(it);

            if (errors.any())
            {
                std::cout << "One or more errors occurred:" << std::endl;
                for each(auto& error in errors)
                {
                    std::cout << error.token() << " " << error.symbol() << " - " << error.message() << std::endl;
                }
            }
            else
            {
                auto const dump = libformula::Term<libformula::util::CodeDump>(input.begin(), input.end(), &errors);

                std::cout << "Result: " << result << std::endl;
            }
        }
    } while(input != "");


    return 0;
}

