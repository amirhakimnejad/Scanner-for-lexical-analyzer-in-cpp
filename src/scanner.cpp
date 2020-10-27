/**
 * @file scanner.cpp
 * @author <a href="mailto:a.hakimnejad@mrl-spl.ir">Amirhossein Hakimnejad</a>
 *
 * @date 2018 Nov
 */


#include "scanner.hpp"

using std::cout;
using std::vector;

bool isID(const std::string &str)
{
  if(std::isdigit(str[0]))
    return false;
  int counter = 0;
  if(str[0] == '_')
    counter++;

  for(; counter < str.size(); counter++)
    if(!(isalnum(str[counter])))
      return false;

  return true;
}

bool isComment(const std::string &str)
{
  return str == "/*" || str == "//";
}

bool isDigit(const std::string &str)
{
  return std::all_of(str.begin(), str.end(), ::isdigit);
}

bool isString(const std::string &str)
{
  return str[0] == '"' && str[str.size()-1] == '"';
}

bool isBool(const std::string &str)
{
  return str == "true" || str == "false";
}

bool isLiteral(const std::string &str)
{
  return isDigit(str) || isString(str) || isBool(str);
}

bool isKeyword(const std::string &str)
{
  const vector<std::string> keywords{"int", "float", "auto", "double", "do", "switch", "return"};
  for(const auto& keyword : keywords)
    if (keyword == str)
      return true;

  return false;
}

bool isStatement(const std::string &str)
{
  const vector<std::string> statements{"for", "while"};
  for(const auto& statement : statements)
    if (statement == str)
      return true;

  return false;
}

bool isOperator(const std::string &str)
{
  const vector<std::string> operators{"<", ">", "<=", ">=", "*", "+", "-", "/", "=", "-=", "*=", "+=", "/=", "++", "--", "=="};
  for(const auto& op : operators)
    if (op == str)
      return true;

  return false;
}

bool isSeparator(const std::string &str)
{
  const vector<std::string> Separators{"{", "}", ",", "(", ")", ";"};
  for(const auto& separate : Separators)
    if (separate == str)
      return true;

  return false;
}

bool isNotLegal(const std::string &str)
{
  return str == " " || str == "\n";
}

void printRoleOfToken(const std::string& token)
{
  if(isOperator(token))
    cout << "(operator, " << token << ")";
  else if(isSeparator(token))
    cout << "(separator, " << token << ")";
  else if(isKeyword(token))
    cout << "(keyword, " << token << ")";
  else if(isStatement(token))
    cout << "(statement, " << token << ")";
  else if(isLiteral(token))
    cout << "(literal, " << token << ")";
  else if(isID(token))
    cout << "(identifier, " << token << ")";
  else if(isComment(token))
    cout << "(comment, " << token << ")";
  else
    throw std::runtime_error(token);
}

void lexicalAnalyze(const std::string &nameOfFile)
{
  char ch;
  std::string buffer;
  std::fstream file(nameOfFile, std::fstream::in);

  if (!file.is_open())
  {
    cout << "error while opening the file\n";
    exit(0);
  }

  bool miltiCm = false, singleCm = false;
  while (file >> std::noskipws >> ch)
  {
    if(singleCm || miltiCm)
    {
      if(singleCm && ch == '\n')
        singleCm = false;

      if(miltiCm && ch == '*')
      {
        file >> ch;
        if(ch == EOF)
          break;

        if(ch == '/')
          miltiCm = false;
      }
      continue;
    }

    if(ch == '/')
    {
      std::string comm(1, ch);
      file >> ch;
      if(ch == EOF)
      {
        printRoleOfToken(comm);
        break;
      }

      if(ch == '*')
      {
        miltiCm = true;
        comm += ch;
      }
      else if(ch == '/')
      {
        singleCm = true;
        comm += ch;
      }
      if(miltiCm || singleCm)
      {
        printRoleOfToken(comm);
        continue;
      }
    }

    if(isNotLegal(std::string(1, ch)))
    {
      if(!buffer.empty())
      {
        printRoleOfToken(buffer);
        buffer = "";
      }
      continue;
    }

    if(isOperator(std::string(1, ch)) && !isOperator(buffer))
    {
      if(!buffer.empty())
      {
        printRoleOfToken(buffer);
        buffer = "";
      }
    }

    if(!isOperator(std::string(1, ch)) && isOperator(buffer))
    {
      printRoleOfToken(buffer);
      buffer = "";
    }

    if(isSeparator(std::string(1, ch)))
    {
      if(!buffer.empty())
      {
        printRoleOfToken(buffer);
        buffer = "";
      }
      if(isSeparator(std::string(1, ch)))
      {
        printRoleOfToken(std::string(1, ch));
        continue;
      }
    }
    buffer += ch;
  }
  file.close();
}