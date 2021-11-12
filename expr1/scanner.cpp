/*
 * @Author       : Gehrychiang
 * @LastEditTime : 2021-11-12 19:04:50
 * @Website      : www.yilantingfeng.site
 * @E-mail       : gehrychiang@aliyun.com
 * @ProbTitle    : (记得补充题目标题)
 */
/*
TODO
// auto	    声明自动变量
// short	声明短整型变量或函数
// int	    声明整型变量或函数
// long	    声明长整型变量或函数
// float	声明浮点型变量或函数
// double	声明双精度变量或函数
// char	    声明字符型变量或函数
// const	声明只读变量
// unsigned	声明无符号类型变量或函数
// signed	声明有符号类型变量或函数
// void	    声明函数无返回值或无参数，声明无类型指针
// if	    条件语句
// else	    条件语句否定分支（与 if 连用）
// switch	用于开关语句
// case	    开关语句分支
// for	    一种循环语句
// do	    循环语句的循环体
// while	循环语句的循环条件
// continue	结束当前循环，开始下一轮循环
// break	跳出当前循环
// default	开关语句中的“其他”分支
// sizeof	计算数据类型长度
// return	子程序返回语句（可以带参数，也可不带参数）循环条件

identifier <- keyword
number(oct,dec,hex)
// + plus ++ self_plus += plus_eq
// - minus -- self_minus -= minus_eq
// * times *= times_eq
// / over /= over_eq
// = eq == logi_eq
// < lt <= lt_eq
// > gt >= gt_eq
// % mod
// & bit_and && logi_and
// | bit_or || logi_or
// ! not
// ?/: triple opr( remain to be decided)
// \ slash
// ; semi
// ( lpar
// ) rpar
// [ lbrk
// ] rbrk
// { lbrc
// } rbrc
// ' apostrophe
// " double_quotation


*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include "./bprinter/table_printer.h"
using bprinter::TablePrinter;
#define keyword_count 26
char reserved_keyword[][20] = {"auto", "break", "case", "char", "continue",
                               "const", "default", "do", "double", "else",
                               "float", "for", "if", "int", "long", "return",
                               "struct", "string", "switch", "short", "signed", "sizeof",
                               "unsigned", "void", "while", "NULL"};

typedef enum
{
    Start_state,
    In_ID_state,
    ID_end_state,

    plus_state,
    minus_state,
    times_state,
    eq_state,

    comment_begin_state,
    In_comment_state,
    comment_end_state,

    exclamation_state,
    apostrophe_begin_state,
    apostrophe_slash_state,
    apostrophe_end_state,
    double_quotation_begin_state,
    double_quotation_slash_state,
    bit_and_state,
    bit_or_state,

    zero_state,
    non_zero_state,
    In_dec_state,
    In_oct_state,
    In_hex_state,
    In_real_state,
    In_exp_state,

    Err_state,
    End_state,
} state_type;

typedef enum
{
    error,
    keyword,
    identifier,
    dec_integer,
    oct_integer,
    hex_integer,
    real_number,
    comment,
    single_delimiter,
    algo_operator,
    arrow_operator,
    self_operator,
    assignment_operator,
    bit_operator,
    logi_operator,
    compound_assignment_operator,
    single_relational_operator,
    double_relational_operator,
    char_const,
    string_const,
} word_type;

char friendly_wordtype[][105] = {
    "error",
    "keyword",
    "identifier",
    "dec_integer",
    "oct_integer",
    "hex_integer",
    "real_number",
    "comment",
    "single_delimiter",
    "algo_operator",
    "arrow_operator",
    "self_operator",
    "assignment_operator",
    "bit_operator",
    "logi_operator",
    "compound_assignment_operator",
    "single_relational_operator",
    "double_relational_operator",
    "char_const",
    "string_const"};

typedef enum
{
    unknown,
    letter,
    hex_digit,
    oct_digit,
    digit,
    zero,
    hex_label,
    real_number_label,
    underline,
    exponent,
    decimal_point,
    asterisk,
    slash,
    interrogation_mark,
    colon,
    backslash,
    comma,
    space,

    plus,
    minus,
    times,
    over,
    eq,
    lt,
    gt,
    mod,
    bit_and,
    bit_or,
    exclamation,
    semicolon,
    left_bracket,
    right_bracket,
    left_brace,
    right_brace,
    left_parenthesis,
    right_parenthesis,
    apostrophe,
    double_quotation,
    eof
} character_type;

word_type get_word_type(char *str)
{
    // printf("debug::%s\n\n",str);
    int found = 0;
    for (int i = 0; i < keyword_count; i++)
    {
        if (strcmp(str, reserved_keyword[i]) == 0)
        {
            found = 1;
            break;
        }
    }

    if (found)
        return keyword;
    else
        return identifier;
}
character_type get_character_type(char ch, state_type cur_state)
{
    if (cur_state == zero_state && ch == 'x')
        return hex_label;
    else if ((cur_state == zero_state || cur_state == In_oct_state) && (ch >= '0' && ch <= '7'))
        return oct_digit;
    else if (cur_state == Start_state && ch == '0')
        return zero;
    else if ((cur_state == In_dec_state || cur_state == In_real_state) && (ch == 'e' || ch == 'E')) //在十进制整数下处理指数
        return exponent;
    else if (cur_state == In_hex_state && (ch >= '0' && ch <= '9' || (ch >= 'a' && ch <= 'e') || (ch >= 'A' && ch <= 'E'))) //在十六进制下处理a-e
        return hex_digit;
    else if (ch >= 'A' && ch <= 'Z' || ch >= 'a' && ch <= 'z')
        return letter;
    else if (ch >= '0' && ch <= '9')
        return digit;
    else if (ch == '_')
        return underline;
    else if (ch == '.')
        return decimal_point;
    else if (ch == '*')
        return times;
    else if (ch == '%')
        return mod;
    else if (ch == '/')
        return slash;
    else if (ch == '\\')
        return backslash;
    else if (ch == ':')
        return colon;
    else if (ch == '[')
        return left_bracket;
    else if (ch == ']')
        return right_bracket;
    else if (ch == ',')
        return comma;
    else if (ch == ';')
        return semicolon;
    else if (ch == ' ')
        return space;
    else if (ch == '+')
        return plus;
    else if (ch == '-')
        return minus;
    else if (ch == '=')
        return eq;
    else if (ch == '>')
        return gt;
    else if (ch == '<')
        return lt;
    else if (ch == '!')
        return exclamation;
    else if (ch == '\'')
        return apostrophe;
    else if (ch == '\"')
        return double_quotation;
    else if (ch == '(')
        return left_parenthesis;
    else if (ch == ')')
        return right_parenthesis;
    else if (ch == '{')
        return left_brace;
    else if (ch == '}')
        return right_brace;
    else if (ch == '&')
        return bit_and;
    else if (ch == '|')
        return bit_or;
    else
        return unknown;
}
character_type get_next_character_type(char *straddr, int strlength, int cur_pos, state_type cur_state)
{
    if (cur_pos >= strlength)
        return eof;
    else
        return get_character_type(straddr[cur_pos], cur_state);
}
char *scopy_word(char *straddr, int l, int r)
{
    char *tmpstr = (char *)calloc(256, sizeof(char));
    for (int i = l; i < r; i++)
    {
        tmpstr[i - l] = straddr[i];
    }
    return tmpstr;
}

char *RecogniteWordByDFA(char *straddr, int strlength, int *start_pos, word_type *ret)
{

    state_type cur_state = Start_state;
    word_type wordtype = error;

    int cur_pos = *start_pos;
    char *tmpstr;

    while (cur_state != End_state)
    {
        switch (cur_state)
        {
        case Start_state:
            switch (get_next_character_type(straddr, strlength, cur_pos, cur_state))
            {
            //number
            case zero:
                cur_state = zero_state;
                break;
            case digit: //actually non zero
                cur_state = In_dec_state;
                break;

            //other
            case letter:
                cur_state = In_ID_state;
                break;
            case underline:
                cur_state = In_ID_state;
                break;
            case plus:
                cur_state = plus_state;
                break;
            case minus:
                cur_state = minus_state;
                break;
            case times:
                cur_state = times_state;
                break;
            case eq:
                cur_state = eq_state;
                break;
            case lt:
                cur_state = eq_state;
                break;
            case gt:
                cur_state = eq_state;
                break;
            case slash:
                cur_state = comment_begin_state;
                break;
            case bit_and:
                cur_state = bit_and_state;
                break;
            case bit_or:
                cur_state = bit_or_state;
                break;
            case exclamation:
                cur_state = exclamation_state;
                break;
            case apostrophe:
                cur_state = apostrophe_begin_state;
                break;
            case double_quotation:
                cur_state = double_quotation_begin_state;
                break;

            //direct trans
            case space:
                cur_state = End_state;
                wordtype = single_delimiter;
                break;
            case semicolon:
                cur_state = End_state;
                wordtype = single_delimiter;
                break;

            case decimal_point:
                cur_state = End_state;
                wordtype = single_delimiter;
                break;
            case mod:
                cur_state = End_state;
                wordtype = single_delimiter;
                break;
            case colon:
                cur_state = End_state;
                wordtype = single_delimiter;
                break;

            case comma:
                cur_state = End_state;
                wordtype = single_delimiter;
                break;
            case left_brace:
                cur_state = End_state;
                wordtype = single_delimiter;
                break;
            case left_bracket:
                cur_state = End_state;
                wordtype = single_delimiter;
                break;
            case left_parenthesis:
                cur_state = End_state;
                wordtype = single_delimiter;
                break;
            case right_brace:
                cur_state = End_state;
                wordtype = single_delimiter;
                break;
            case right_bracket:
                cur_state = End_state;
                wordtype = single_delimiter;
                break;
            case right_parenthesis:
                cur_state = End_state;
                wordtype = single_delimiter;
                break;
            default:
                cur_state = Err_state;
                break;
            }
            cur_pos++;
            break;

        case In_ID_state:
            if (get_next_character_type(straddr, strlength, cur_pos, cur_state) != letter && get_next_character_type(straddr, strlength, cur_pos, cur_state) != digit && get_next_character_type(straddr, strlength, cur_pos, cur_state) != underline)
            {
                cur_state = ID_end_state;
            }
            else
            {
                cur_state = In_ID_state;
                cur_pos++;
            }
            break;

        case ID_end_state:
            tmpstr = scopy_word(straddr, *start_pos, cur_pos);
            wordtype = get_word_type(tmpstr);
            cur_state = End_state;
            break;

        case plus_state:
            if (get_next_character_type(straddr, strlength, cur_pos, cur_state) == plus)
            {
                wordtype = self_operator;
                cur_state = End_state;
                cur_pos++;
            }
            else if (get_next_character_type(straddr, strlength, cur_pos, cur_state) == eq)
            {
                wordtype = compound_assignment_operator;
                cur_state = End_state;
                cur_pos++;
            }
            else
            {
                wordtype = algo_operator;
                cur_state = End_state;
            }
            break;

        case minus_state:
            if (get_next_character_type(straddr, strlength, cur_pos, cur_state) == minus)
            {
                wordtype = self_operator;
                cur_state = End_state;
                cur_pos++;
            }
            else if (get_next_character_type(straddr, strlength, cur_pos, cur_state) == gt)
            {
                wordtype = arrow_operator;
                cur_state = End_state;
                cur_pos++;
            }
            else if (get_next_character_type(straddr, strlength, cur_pos, cur_state) == eq)
            {
                wordtype = compound_assignment_operator;
                cur_state = End_state;
                cur_pos++;
            }
            else
            {
                wordtype = algo_operator;
                cur_state = End_state;
            }
            break;

        case times_state:
            if (get_next_character_type(straddr, strlength, cur_pos, cur_state) == eq)
            {
                wordtype = compound_assignment_operator;
                cur_state = End_state;
                cur_pos++;
            }
            else
            {
                wordtype = algo_operator;
                cur_state = End_state;
            }
            break;

        case eq_state:
            if (get_next_character_type(straddr, strlength, cur_pos, cur_state) == eq)
            {
                wordtype = double_relational_operator;
                cur_state = End_state;
                cur_pos++;
            }
            else
            {
                wordtype = assignment_operator;
                cur_state = End_state;
            }
            break;

        case comment_begin_state:
            if (get_next_character_type(straddr, strlength, cur_pos, cur_state) == times)
            {
                cur_state = In_comment_state;
                cur_pos++;
            }
            else if (get_next_character_type(straddr, strlength, cur_pos, cur_state) == eq)
            {
                wordtype = compound_assignment_operator;
                cur_state = End_state;
                cur_pos++;
            }
            else
            {
                wordtype = algo_operator;
                cur_state = End_state;
            }
            break;

        case In_comment_state:
            if (get_next_character_type(straddr, strlength, cur_pos, cur_state) == times)
            {
                cur_state = comment_end_state;
                cur_pos++;
            }
            else if (get_next_character_type(straddr, strlength, cur_pos, cur_state) == eof)
            {
                wordtype = comment;
                cur_state = End_state;
            }
            else
            {
                cur_state = In_comment_state;
                cur_pos++;
            }
            break;

        case comment_end_state:
            if (get_next_character_type(straddr, strlength, cur_pos, cur_state) == slash)
            {
                wordtype = comment;
                cur_state = End_state;
                cur_pos++;
            }
            else if (get_next_character_type(straddr, strlength, cur_pos, cur_state) == eof)
            {
                wordtype = comment;
                cur_state = End_state;
            }
            else
            {
                cur_state = In_comment_state;
                cur_pos++;
            }
            break;
        case exclamation_state:
            if (get_next_character_type(straddr, strlength, cur_pos, cur_state) == eq)
            {
                wordtype = double_relational_operator;
                cur_state = End_state;
                cur_pos++;
            }
            else
            {
                wordtype = single_relational_operator;
                cur_state = End_state;
            }
            break;

        case apostrophe_begin_state:
            if (get_next_character_type(straddr, strlength, cur_pos, cur_state) == backslash)
            {
                cur_state = apostrophe_slash_state;
                cur_pos++;
            }
            else if (get_next_character_type(straddr, strlength, cur_pos, cur_state) == apostrophe)
            {
                cur_state = Err_state;
            }
            else if (get_next_character_type(straddr, strlength, cur_pos, cur_state) == eof)
            {
                cur_state = Err_state;
            }
            else
            {
                cur_state = apostrophe_end_state;
                cur_pos++;
            }
            break;

        case apostrophe_slash_state:
            if (get_next_character_type(straddr, strlength, cur_pos, cur_state) == eof)
            {
                cur_state = Err_state;
            }
            else
            {
                cur_state = apostrophe_end_state;
                cur_pos++;
            }
            break;

        case apostrophe_end_state:
            if (get_next_character_type(straddr, strlength, cur_pos, cur_state) == apostrophe)
            {
                wordtype = char_const;
                cur_state = End_state;
                cur_pos++;
            }
            else
            {
                cur_state = Err_state;
            }
            break;

        case double_quotation_begin_state:
            if (get_next_character_type(straddr, strlength, cur_pos, cur_state) == backslash)
            {
                cur_state = double_quotation_slash_state;
                cur_pos++;
            }
            else if (get_next_character_type(straddr, strlength, cur_pos, cur_state) == double_quotation)
            {
                wordtype = string_const;
                cur_state = End_state;
                cur_pos++;
            }
            else if (get_next_character_type(straddr, strlength, cur_pos, cur_state) == eof)
            {
                cur_state = Err_state;
            }
            else
            {
                cur_state = double_quotation_begin_state;
                cur_pos++;
            }
            break;
        case double_quotation_slash_state:
            if (get_next_character_type(straddr, strlength, cur_pos, cur_state) == eof)
            {
                cur_state = Err_state;
            }
            else
            {
                cur_state = double_quotation_begin_state;
                cur_pos++;
            }
            break;
        case bit_and_state:
            if (get_next_character_type(straddr, strlength, cur_pos, cur_state) == bit_and)
            {
                wordtype = logi_operator;
                cur_state = End_state;
                cur_pos++;
            }
            else
            {
                wordtype = bit_operator;
                cur_state = End_state;
            }
            break;
        case bit_or_state:
            if (get_next_character_type(straddr, strlength, cur_pos, cur_state) == bit_or)
            {
                wordtype = logi_operator;
                cur_state = End_state;
                cur_pos++;
            }
            else
            {
                wordtype = bit_operator;
                cur_state = End_state;
            }
            break;
        case zero_state:
            if (get_next_character_type(straddr, strlength, cur_pos, cur_state) == hex_label)
            {
                cur_state = In_hex_state;
                cur_pos++;
            }
            else if (get_next_character_type(straddr, strlength, cur_pos, cur_state) == decimal_point)
            {
                cur_state = In_real_state;
                cur_pos++;
            }
            else if (get_next_character_type(straddr, strlength, cur_pos, cur_state) == oct_digit)
            {
                cur_state = In_oct_state;
                cur_pos++;
            }
            else
            {
                wordtype = dec_integer;
                cur_state = End_state;
            }
            break;
        case In_oct_state:
            if (get_next_character_type(straddr, strlength, cur_pos, cur_state) == oct_digit)
            {
                cur_state = In_oct_state;
                cur_pos++;
            }
            else
            {
                wordtype = oct_integer;
                cur_state = End_state;
            }
            break;

        case In_hex_state:
            if (get_next_character_type(straddr, strlength, cur_pos, cur_state) == hex_digit)
            {
                cur_state = In_hex_state;
                cur_pos++;
            }
            else
            {
                wordtype = hex_integer;
                cur_state = End_state;
            }
            break;

        case In_dec_state:
            if (get_next_character_type(straddr, strlength, cur_pos, cur_state) == digit)
            {
                cur_state = In_dec_state;
                cur_pos++;
            }
            else if (get_next_character_type(straddr, strlength, cur_pos, cur_state) == decimal_point)
            {
                cur_state = In_real_state;
                cur_pos++;
            }
            else if (get_next_character_type(straddr, strlength, cur_pos, cur_state) == exponent)
            {
                cur_state = In_exp_state;
                cur_pos++;
            }
            else
            {
                wordtype = dec_integer;
                cur_state = End_state;
            }
            break;

        case In_real_state:
            if (get_next_character_type(straddr, strlength, cur_pos, cur_state) == digit)
            {
                cur_state = In_real_state;
                cur_pos++;
            }
            else if (get_next_character_type(straddr, strlength, cur_pos, cur_state) == exponent)
            {
                cur_state = In_exp_state;
                cur_pos++;
            }
            else
            {
                wordtype = real_number;
                cur_state = End_state;
            }
            break;

        case In_exp_state:
            if (get_next_character_type(straddr, strlength, cur_pos, cur_state) == digit)
            {
                cur_state = In_exp_state;
                cur_pos++;
            }
            else
            {
                wordtype = real_number;
                cur_state = End_state;
            }
            break;

        case Err_state:
            wordtype = error;
            cur_state = End_state;
            break;
        default:
            wordtype = error;
            cur_state = End_state;
            break;
        }
    }

    tmpstr = scopy_word(straddr, *start_pos, cur_pos);

    *start_pos = cur_pos;
    // printf("%s ", tmpstr);
    *ret = wordtype;
    return tmpstr;
}
char *read_program_and_trim()
{
    char *input_str = (char *)calloc(100005, sizeof(char));
    int p = 0;
    FILE *fp = fopen("inputTest.c", "r");
    while (fgets(input_str + p, 10000, fp) != NULL)
    {
        p = strlen(input_str);
        input_str[p] = ' ';
        input_str[p + 1] = '\0';
        p++;
    }
    for (int i = 0; i < strlen(input_str); i++)
    {
        if (input_str[i] == '\n' || input_str[i] == '\r')
        {
            input_str[i] = ' ';
        }
    }
    for (int i = 0; i < strlen(input_str); i++)
    {
        if (input_str[i] == ' ')
        {
            int l = i;
            int r = i;
            while (r + 1 < strlen(input_str) && input_str[r + 1] == ' ')
            {
                r++;
            }
            //[l,r] space
            for (int j = r + 1; j < strlen(input_str) + 1; j++)
            {
                input_str[j - r + l] = input_str[j];
            }
            i = l;
        }
    }
    return input_str;
}
int main()
{
    //recognize id
    char *input_str = read_program_and_trim();
    int curpos = 0;
    word_type res;
    char *out = NULL;
    TablePrinter tp(&std::cout);
    tp.set_flush_left();
    while (curpos < strlen(input_str))
    {
        if (out == NULL)
        {
            tp.AddColumn("String", 20);
            tp.AddColumn("Category", 30);
            tp.PrintHeader();
        }
        out = RecogniteWordByDFA(input_str, strlen(input_str), &curpos, &res);
        if (out[0] != '\n')
            tp << out;
        else
            tp << "";
        tp << friendly_wordtype[res];
        tp.PrintFooter();
    }

    return 0;
}