/*
 * @Author       : Gehrychiang
 * @LastEditTime : 2021-10-29 22:06:08
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
#define keyword_count 24
char reserved_keyword[][20] = {"auto", "break", "case", "char", "continue",
                               "const", "default", "do", "double", "else",
                               "float", "for", "if", "int", "long", "return",
                               "struct", "switch", "short", "signed", "sizeof",
                               "unsigned", "void", "while"};

typedef enum
{
    Start_state,
    In_ID_state,
    ID_end_state,

    plus_state,
    minus_state,

    In_comment_state,
    In_dec_state,
    In_oct_state,
    In_hex_state,

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
    self_operator,
    compound_assignment_operator,
    single_relational_operator,
    double_relational_operator,
    char_const,
    string_const,
    eof
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
    "self_operator",
    "compound_assignment_operator",
    "single_relational_operator",
    "double_relational_operator",
    "char_const",
    "string_const",
    "eof"};

typedef enum
{
    unknown,
    letter,
    hex_digit,
    digit,
    underline,
    exponent,
    decimal_point,
    asterisk,
    slash,
    // interrogation_mark,
    // colon,
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
    not,
    semi,
    left_bracket,
    right_bracket,
    left_brace,
    right_brace,
    left_parenthesis,
    right_parenthesis,
    apostrophe,
    double_quotation
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
    if (cur_state == In_dec_state && (ch == 'e' || ch == 'E')) //在十进制整数下处理指数
        return exponent;
    else if (cur_state == In_hex_state && ((ch >= 'a' && ch <= 'e') || (ch >= 'A' && ch <= 'E'))) //在十六进制下处理a-e
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
    // else if (ch == ':')
    //     return colon;
    else if (ch == '[')
        return left_bracket;
    else if (ch == ']')
        return right_bracket;
    else if (ch == ',')
        return comma;
    else if (ch == 32) /* 空格 ASCII码值为32*/
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
        return not ;
    else if (ch == '\'') /* 单引号*/
        return apostrophe;
    else if (ch == '\"') /* 双引号*/
        return double_quotation;
    else if (ch == '(')
        return left_parenthesis;
    else if (ch == ')')
        return right_parenthesis;
    else if (ch == '{')
        return left_brace;
    else if (ch == '}')
        return right_brace;
    else
        return unknown;
    return 0;
}
character_type get_next_character_type(char *straddr, int strlength, int cur_pos, state_type cur_state)
{
    if (cur_pos >= strlength)
        return eof;
    else
        return get_character_type(straddr[cur_pos], cur_state);
}
void scopy_word(char *tmpstr, char *straddr, int l, int r)
{
    memset(tmpstr, sizeof(tmpstr), 0);
    for (int i = l; i < r; i++)
    {
        tmpstr[i - l] = straddr[i];
    }
}

word_type RecogniteWordByDFA(char *straddr, int strlength, int *start_pos)
{

    state_type cur_state = Start_state;
    word_type wordtype = error;

    int cur_pos = *start_pos;
    char tmpstr[256];

    while (cur_state != End_state)
    {
        switch (cur_state)
        {
        case Start_state:
            switch (get_next_character_type(straddr, strlength, cur_pos, cur_state))
            {
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
                cur_pos++;
                cur_state = In_ID_state;
            }

            break;

        case ID_end_state:
            scopy_word(tmpstr, straddr, *start_pos, cur_pos);
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
            scopy_word(tmpstr, straddr, *start_pos, cur_pos);

            break;

        case minus_state:
            if (get_next_character_type(straddr, strlength, cur_pos, cur_state) == minus)
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
            scopy_word(tmpstr, straddr, *start_pos, cur_pos);

            break;
        case Err_state:
            scopy_word(tmpstr, straddr, *start_pos, cur_pos);
            wordtype = error;
            cur_state = End_state;
            break;
        default:
            wordtype = error;
            cur_state = End_state;
            break;
        }
    }
    *start_pos = cur_pos;
    printf("%s ", tmpstr);
    return wordtype;
}
int main()
{
    //recognize id
    char input_str[10005];
    gets(input_str);
    int curpos = 0;
    while (curpos < strlen(input_str))
    {
        word_type res = RecogniteWordByDFA(input_str, strlen(input_str), &curpos);
        printf("%s\n", friendly_wordtype[res]);
    }

    return 0;
}