%{
#include <string.h>
#include <dirent.h>
#include <iostream>
extern int yylex(void);
using namespace std;

#undef yywrap
//---------  INSERT ANY CODE CALLED BY THE LEX RULES HERE --------
void DowncaseAndPrint(const char Str[])
{
    char Ch;

    for (int i = 0 ; i < (int)strlen(Str); i++)
    {
        if ('A' <= Str[i] && Str[i] <= 'Z')
        {
            Ch = (char)('a' + Str[i] - 'A');
            fprintf(yyout, "%c", Ch);
        }
        else
            fprintf(yyout, "%c", Str[i]);
    }
    fprintf(yyout, "\n");
}
void ExtractHtmlAttributeContents(const char Str[], const std::string match)
{
    int p1 = 0, p2 = 0;

    // Searching for the pattern (e.g., 'alt="')
    while (p1 < strlen(Str) && p2 < match.size())
    {
        if (Str[p1] == match[p2])
        {
            p1++;
            p2++;
        }
        else
        {
            p1++;
            p2 = 0;  // Reset p2 if it's not a match
        }
    }

    if (p2 == match.size())  // If we found the match
    {
        string curr = "";  // Use a std::string to store the current word

        // Process the content of the attribute
        while (p1 < strlen(Str) && Str[p1] != '"')
        {
            if (Str[p1] == ' ')  // When encountering a space, print the accumulated word
            {
                DowncaseAndPrint(curr.c_str());  // Print the current word
                curr.clear();
            }
            else
            {
                curr += Str[p1];
            }
            p1++;
        }

        if (!curr.empty())
        {
            DowncaseAndPrint(curr.c_str());
        }
    }
}
void HandleHTMLOpenTags(const char Str[])
{
    ExtractHtmlAttributeContents(Str, "alt=\"");
    ExtractHtmlAttributeContents(Str, "contents=\"");
}

// Example of a function called inside yywrap
void ProcessDocumentHashtable()
{
    cout << endl;
    cout << "Document hashtable should have been filled.\n";
    cout << "Time to deal with its contents.\n";
}

void MatchUrls() {
    fprintf(yyout, "%s", yytext);  
    fprintf(yyout, "\n");
}

void MatchInteger() {
    fprintf(yyout, "%s", yytext);  
    fprintf(yyout, "\n");
}
/*----------------------------------------------------------*/
/*Section of code which specifies lex substitution patterns */
/*----------------------------------------------------------*/

%}
HTML_COMMENT      <!--(.|\n)*?-->
HTML_OPENING_TAGS <[a-zA-Z][^>\/]*>
HTML_CLOSING_TAGS <\/[a-zA-Z][^>\/]*>
URLS              https?:\/\/[a-zA-Z0-9\-\.]+\.[a-zA-Z]{2,6}(\/[a-zA-Z0-9\-._~\/]*)?
WORD              [A-Za-z][a-z]*
FLOAT             [0-9]+.[0-9]+
INTEGER           [0-9]+

%%
{HTML_COMMENT}        ;   // Ignore HTML comments (e.g., <!-- comment -->)
{HTML_CLOSING_TAGS}   ;   // Ignore Html closing tags
{URLS}  { MatchUrls(); };  
{HTML_OPENING_TAGS}  {HandleHTMLOpenTags(yytext);}; 
{FLOAT} ;  // Ignore floats
{INTEGER} { MatchInteger(); };  // Process integers
{WORD}  { if (strlen(yytext) > 1) DowncaseAndPrint(yytext); };  // Process words with length > 1
[\n\r\t ] ;  // Ignore whitespace
. ;  // Ignore any other characters
%%

DIR *InputDirPtr = NULL;
string InDirname;
string OutDirname;
bool OutputFileIsOpen = false;

int yywrap()
{
    struct dirent *InputDirEntryPtr;
    string InFilename;
    string OutFilename;

    if (yyin != NULL)
    {
        fclose(yyin);
        yyin = NULL;
        ProcessDocumentHashtable();
    }

    if (OutputFileIsOpen)
    {
        fclose(yyout);
        OutputFileIsOpen = false;
    }

    do
    {
        InputDirEntryPtr = readdir(InputDirPtr);
    } while ((InputDirEntryPtr != NULL) &&
             (InputDirEntryPtr->d_name[0] == '.'));

    if (InputDirEntryPtr != NULL)
    {
        InFilename = InDirname + '/' + InputDirEntryPtr->d_name;
        cout << "\nProcessing file " << InFilename << endl;
        yyin = fopen(InFilename.c_str(), "r");

        if (yyin == NULL)
            perror(InFilename.c_str());

        OutFilename = OutDirname + '/' + InputDirEntryPtr->d_name;
        size_t pos = OutFilename.rfind(".html");
        if (pos != string::npos)
        {
            OutFilename.replace(pos, 5, ".out"); // Replace ".html" with ".out"
        }
        else
        {
            OutFilename += ".out"; // If no ".html" found, just append ".out"
        }
        yyout = fopen(OutFilename.c_str(), "w");

        if (yyout == NULL)
            perror(OutFilename.c_str());
        else
            OutputFileIsOpen = true;
    }
    return (yyin == NULL);
}

int main(int argc, char **argv)
{
    DIR *OutputDirPtr = NULL;

    if (argc != 3)
    {
        cerr << "Incorrect number of arguments.\n";
        return (1);
    }

    InDirname = argv[1];
    OutDirname = argv[2];
    InputDirPtr = opendir(InDirname.c_str());

    if (!InputDirPtr)
        cerr << "Unable to open input directory: " << InDirname << endl;
    else
    {
        OutputDirPtr = opendir(OutDirname.c_str());
        if (!OutputDirPtr)
        {
            string Command = "mkdir -p " + OutDirname;
            system(Command.c_str());
        }

        yyin = NULL;
        yyout = NULL;

        yywrap();
        yylex();

        (void)closedir(InputDirPtr);
        (void)closedir(OutputDirPtr);
    }
    cout << "Done tokenizing.  Good place to write the dict and post files." << endl;
}
