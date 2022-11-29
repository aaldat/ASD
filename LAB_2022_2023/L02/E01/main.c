//	Alessio Flamini s268460

#include <stdio.h>
#include <stdlib.h>

int gcd(int num1, int num2);

int main()
{
	int a, b, tmp, mcd;
	printf("Inserisci il primo numero: ");
	scanf("%d", &a);
	printf("Inserisci il secondo numero: ");
	scanf("%d", &b);

	if(a < b) {
		tmp = a;
		a = b;
		b = tmp;
	}

	if (a % 2 == 0 && b % 2 == 0) {
		mcd = 2 * gcd(a/2, b/2);
	} else if (a % 2 != 0 && b % 2 == 0) {
		mcd = gcd(a, b/2);
	} else if (a % 2 != 0 && b % 2 != 0) {
		mcd = gcd((a - b) / 2, b);
	} else {
		mcd = gcd(a/2, b);
	}

	printf("Il massimo comune divisore tra %d e %d e' %d.", a, b, mcd);

	return 0;
}

int gcd(int num1, int num2)
{
	int resto;

	resto = num1 % num2;
	while (resto > 0) {
		num1 = num2;
		num2 = resto;
		resto = num1 % num2;
	}

	return num2;
}
