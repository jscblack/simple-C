/*comment here*/
struct node
{
    int val;
};
int main(void)
{
    struct node *a;
    a->val = 0x7fffff;
    if (a->val & 1 != 0)
    {
        printf("hello, world\n");
    }
    else
    {
        printf("%d", sizeof(*a));
    }
    return 0;
}