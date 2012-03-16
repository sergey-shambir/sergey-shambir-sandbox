#ifndef SCANNER_HPP_eb01b142_2f9f_477e_ab52_2941e478f5e5
#define SCANNER_HPP_eb01b142_2f9f_477e_ab52_2941e478f5e5

#include "SourceStream.hpp"
#include "Token.hpp"

class Scanner
{
public:
    explicit Scanner(std::string const& source);

    /**
      returns next token, always casts identifiers to lower case.
      */
    Token scan();

private:
    Token generateToken(Token::Type type) const;
    TokenPos streamPos() const;

    SourceStream _stream;
    TokenPos _currentTokenStart;
};

#endif // SCANNER_HPP_eb01b142_2f9f_477e_ab52_2941e478f5e5
