#include<stdio.h>
#include<strings.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#define TIGER "HiddenTiger"
#define SPRAVKA_O_FUNCCIYACH "0 - Вывод текста после первичной обработки(удаление повторяющихся предложений.\n1 - Удалить все символы в начале и в конце предложения так, чтобы в итоге первый и последний символы предложений были различны.\n2 - Отсортировать все слова в предложении в лексикографическом порядке.\n3 - Удалить все предложения, в которых хотя бы одно слово является палиндромом.\n4 - Вывести все предложения, в которых присутствует слово HiddenTiger, но при этом не являющееся первым словом предложения.\n"


char** input_text(int* number_of_sent) 
{
	int BASE_WIDTH = 10, BASE_HEIGHT = 10;
	char** our_text = (char**)malloc(BASE_HEIGHT * sizeof(char*));
	char* sentence = (char*)malloc(BASE_WIDTH * sizeof(char));
	char alpha;
	int sent_count = *number_of_sent;
	int length_of_sent = 0;
	char TwoLastEnters[2];//Создание массива из двух символов, проверяющего парность символа переноса строки.
	our_text[sent_count] = sentence;
	while (1) 
        {
		alpha = getchar();
		TwoLastEnters[0] = TwoLastEnters[1];
		TwoLastEnters[1] = alpha;
		if (TwoLastEnters[0] == '\n' && TwoLastEnters[1] == '\n') 
                {
			our_text[sent_count][length_of_sent - 1] = 0;
			if (length_of_sent - 2 >= 0) sent_count += 1;
			*number_of_sent = sent_count;
			return our_text;//Возвращение введенного текста после двойного энтера
		}

		if ((length_of_sent == 0 && alpha == '\t')||(length_of_sent == 0 && alpha == '\n')||(length_of_sent == 0 && alpha == ' ')||(length_of_sent == 0 && alpha == '.'))
		continue;//Игнорирование пробелов, табов, символов переноса в начале предложения
		if (length_of_sent > BASE_WIDTH - 2) 
                {
			BASE_WIDTH += 10;
			sentence = (char*)realloc(sentence, sizeof(char) * BASE_WIDTH);
			our_text[sent_count] = sentence;
			//Перераспределение памяти для предложения в случае переполнения строки
		}
		our_text[sent_count][length_of_sent] = alpha;
		length_of_sent++;

		if (alpha == '.') {
			our_text[sent_count][length_of_sent] = 0;
			int flag = 1;//Создание флага
			for (int i = 0; i < sent_count; i++) 
                        {
				if (!strcasecmp(our_text[i], our_text[sent_count])) 
                                {
					flag = 0;//Инициализация флага нулем в случае равенства предложений.
					break;//Поскольку цикл вложенный, то использование break вместо continue
					//В цикле текущее предложение сравнивается только с каждым введенным ранее. С более поздними сравнение, очевидно, необязательно.
				}
			}
			if (flag==1) 		//В данном условии проверяется значение флага
			sent_count++;		//В случае, если введенное предложение не является копией одного из предыдущих, число предложений увеличивается
			else 				//В противном случае память, выделенная для предложения, освобождается, т.е предложение не сохраняется в памяти.
			free(our_text[sent_count]);

			if (sent_count > BASE_HEIGHT - 2) 
                        {
				BASE_HEIGHT += 10;
				our_text = (char**)realloc(our_text, sizeof(char*) * BASE_HEIGHT);
			}//Перераспределение памяти для массива указателей в случае переполнения

			BASE_WIDTH = 10;
			sentence = (char*)malloc(BASE_WIDTH * sizeof(char));//Выделение памяти для следующего предложения
			our_text[sent_count] = sentence;
			length_of_sent = 0;
		}
	}
}

void base_output_function(char** text, int len) 
{
	for (int i = 0; i < len; i++) 
        {
		printf("%s\n", text[i]);
		free(text[i]);
	}
	free(text);
}
void tiger_output_function(char** text, int len)
{
	for(int i=0;i<len;i++)
        {
		strstr(text[i], TIGER);
		if(strstr(text[i], TIGER) && (strlen(text[i]) - strlen(strstr(text[i], TIGER)))!=0)
		printf("%s\n", text[i]);
		free(text[i]);
	}
	free(text);
}
int is_palindrome(char* word)
{
    int len = strlen(word)-1;
    for(int i=0;i<strlen(word)/2;i++)
    {
        if(tolower(word[i])!=tolower(word[len-i]))
            return 0;//no palindrome
    }
    return 1;//palin.
}
int HasPalindrome(const char* sent)
{
    char copy_sent[strlen(sent)+1];
    strcpy(copy_sent, sent);
    char* word;
    word=strtok(copy_sent, " ,.");
    while(word!=NULL)
    {
        if(is_palindrome(word)==1)
        return 0;//Has palindrome
        word=strtok(NULL, " ,.");
    }
    return 1;//no palindrome in sentence
}
void output_without_palindrome(char** text, int len)
{
        for(int i=0;i<len;i++)
        {
                if(HasPalindrome(text[i])==1)
                        printf("%s\n", text[i]);
                free(text[i]);
        }
        free(text);
}
void without_palindrome_symbols(char* sentence)
{
        int length = strlen(sentence);
        int end = length - 1;
        int i = 0;
        int flag = 0;
        if(sentence[end-1]==' ')
                flag=1;
        while(tolower(sentence[i])==tolower(sentence[end-i-flag-1]))
                i++;
        for(int j = i;j<=end-flag-i-1; j++)
                printf("%c", sentence[j]);
        printf(".\n");
}
void output_without_palindrome_symbols(char** text, int len)
{
	for(int i=0;i<len;i++){
		without_palindrome_symbols(text[i]);
		free(text[i]);
	}
	free(text);
}
void sort_words(char* sentence)
{
	char *words[100];
	int word_count=0;
	char *word=strtok(sentence, " .,-");
	while(word != 0)
	{
		words[word_count++]=word;
		word = strtok(NULL, " .,-");
	}
	for(int i=0; i<word_count;i++){
		for(int j=i+1; j<word_count;j++){
			if(strcasecmp(words[i], words[j])>0){
				char* temp = words[i];
				words[i] = words[j];
				words[j] = temp;
			}
		}
	}

	for(int i = 0; i<word_count;i++)
	printf("%s ", words[i]);
	printf(".\n");
}
void output_in_order(char** text, int len){
	for(int num = 0; num<len; num++){
		sort_words(text[num]);
		free(text[num]);
	}
	free(text);
}


int main() 
{
	printf("Course work for option 4.18, created by Alexander Snigirev.\n");

	int value;
	int count=0;
	char** text;
	scanf("%d", &value);
	switch (value) 
        {
	case 0:
		text = input_text(&count);
		base_output_function(text, count);
		break;
	case 1:
		text = input_text(&count);
        output_without_palindrome_symbols(text, count);                                                                                                ///////////////////////////////
		break;
    case 2:
        text = input_text(&count);
        output_in_order(text, count);                                                                                            ///////////////////////////////
        break;
	case 3:
		text = input_text(&count);
		output_without_palindrome(text, count);
		break;
	case 4:
		text = input_text(&count);
		tiger_output_function(text, count);
		break;
	case 5:
		printf(SPRAVKA_O_FUNCCIYACH);
		break;
	default:
		printf("Error: incorrect function value.");	
	}

	return 0;
}