//
//  main.cpp
//  MarkDown
//
//  Created by 陈鲁同 on 2017/12/8.
//  Copyright © 2017年 陈鲁同. All rights reserved.
//

#include <iostream>
#include <fstream>

#include <vector>
#include "explainer/languageCheck.hpp"
int checkInput(std::string name);

int main(int argc, const char * argv[]) {
    std::unique_ptr<languageCheck> explainer(new languageCheck);
    std::vector<std::string> cmds;
    std::vector<std::string> c;
    std::vector<std::string> outputs;
    std::string buff;
    std::string from;
    std::string dest = "out.html";
    if(argc < 2){
        std::cout<<"Input illegally."<<std::endl;
        return -1;
    }
    if(checkInput(argv[1])){
        from = argv[1];
    }else{
        std::cout<<"Not a legal markdown file."<<std::endl;
        return 1;
    }
    if(argc > 2){
        dest = argv[2];
    }
    
    std::ifstream file;
    file.open(from);
    if(!file){
        std::cout<<"Error file\t" + from<<std::endl;
        return -1;
        
    }
    while(getline(file,buff)){
        c.push_back(buff);
    }
    c.push_back("");
    explainer->setTitle(from);
    explainer->getContent(c);
    explainer->run(c);
    explainer->outputMarkdown(outputs);
    
    std::ofstream out;
    out.open(dest);
    for(auto s:outputs){
        out<<s<<std::endl;
    }
    return 0;
}

int checkInput(std::string name){
    std::smatch sm;
    std::regex_match(name,sm,std::regex(".*[(md)(markdown)(mkd)(mdown)]"));
    return static_cast<int>(sm.size());
};
