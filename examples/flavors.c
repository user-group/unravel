/* Slicing and Dicing example */
int fetch()
{
    int value;
    scanf ("%d",&value);
    return value;
}

main()
{
    int red, green, blue, yellow;
    int sweet,sour,salty,bitter;
    int i;

    red = fetch();
    blue = fetch();
    green = fetch();
    yellow = fetch();

    red = 2*red;
    sweet = red*green;
    sour = 0;
    for (i = 0; i < red; i++)
        sour += green;
    salty = blue + yellow;
    green = green + 1;
    bitter = yellow + green;

    printf ("%d %d %d %d\n",sweet,sour,salty,bitter);
    exit(0);
}
