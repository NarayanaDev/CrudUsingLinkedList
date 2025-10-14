//misc Functions

void removeNewline(char*);

void readText(char *text, int size)
{
	fgets(text, size, stdin);
	removeNewline(text);
}
void readFloat(float *number)
{
	scanf("%f", number);
	getchar();
}
void underline(int length)
{
	for (int counter = 0; counter < length; counter++)
	{
		printf("_");
	}
}
void removeNewline(char *text)
{
	text[strcspn(text, "\n")] = '\0';
}