#pragma once
#include <boost/spirit/include/qi_grammar.hpp>
#include <boost/spirit/include/qi_rule.hpp>

using Iterator = std::string::iterator;

struct SiteGrammar : boost::spirit::qi::grammar<Iterator, std::string()>
{
    SiteGrammar(): base_type(start){
        start = "[Site \"" >> +(boost::spirit::qi::alnum | boost::spirit::qi::space)  >> "\"]";
    }

    boost::spirit::qi::rule<Iterator, std::string()> start;
};
