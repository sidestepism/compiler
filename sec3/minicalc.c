#include <stdio.h>

int main(int argc, char** argv){
	if(argc < 2){
		printf("usage: %s [filename]\n", argv[0]);
		return 1;
	}
	FILE* fp;
	fp = fopen(argv[1], "r");
	
	// error
	if(fp == 0){
		printf("error: opening '%s' failed\n", argv[1]);
		return 1;
	}

	char linebuf[51];
	char* lineptr = linebuf;
	char s;
	int val = 0;
	int ans = 0;
	int cont = 0;

	while(1){
		s = fgetc(fp);
		if(s == ' '){
			// do nothing
			if(cont == 1){
				cont = 2;
			}
			continue;
		}else if(s >= '0' && s <= '9'){
			if(cont == 2){
				printf("syntax error: unexpected digit\n");
				break;
			}
			cont = 1;
			val = val * 10 + (s - '0');
		}else if(s == '+'){
			// printf("value: %d\n", val);
			cont = 0;
			ans += val;
			val = 0;
		}else if(s == '\n'){
			// answer
			// printf("value: %d\n", val);
			cont = 0;
			ans += val;
			val = 0;
			// printf("answer is: %d\n", ans);
			printf("%d\n", ans);
			ans = 0;
		}else if(s == EOF){
			printf("end\n");
			break;
		}else{
			printf("syntax error: unexpected '%c'\n", s);
			break;
		}
	}

}