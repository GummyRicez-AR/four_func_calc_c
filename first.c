#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// this first program is meant to be a text 4-function calculator that also stores memory
// of previous operations used.

typedef struct {
	char variableName;
	float value;
} Variable;

typedef struct {
	float firstOperand;
	char operatorChar;
	float secondOperand;
	float result;
} Equation;

struct EquationListNode {
	struct EquationListNode* prev;
	struct EquationListNode* next;
	Equation* equation;
};

struct EquationListNode* NewEquationNode() {
	struct EquationListNode* newNode = malloc(sizeof(struct EquationListNode));
	newNode->prev = NULL;
	newNode->next = NULL;
	newNode->equation = NULL;
	return newNode;
}

typedef struct {
	struct EquationListNode* head;
	struct EquationListNode* tail;
} EquationList;

EquationList* NewEquationList() {
	EquationList* newList = malloc(sizeof(EquationList));
	newList->head = NULL;
	newList->tail = NULL;
	return newList;
}

//========================================================================

const char spaceBuffer[2] = " ";
const char equalsBuffer[4] = " = ";

//=======================================================================

bool IsWhitespace(char c) {
	return (c == '\n' || c == '\0' || c == ' ' || c == '\t');
}

bool StringIsNumber(char* str) {
	for (int i = 0; *(str + i) != '\0'; i++) {
		if (!isdigit(*(str + i))) {
			return 0;
		}
	}
	return 1;
}

void ClearInputBuffer() {
	int c;
	while ((c = getchar()) != '\n' && c!= EOF);
}

char* RemoveAllWhitespace(char origStr[]) {
	char* newStr = calloc(50, strlen(origStr));
	int currOldIndex = 0;
	int currNewIndex = 0;
	while (currOldIndex < strlen(origStr)) {
		if (!IsWhitespace((origStr[currOldIndex]))) {
			memcpy(&newStr[currNewIndex], &origStr[currOldIndex], 1);
			currNewIndex++;	
		}
		currOldIndex++;
	}
	return newStr;
}

char ReadChar() {
	char resultChar = '\0';
	char inputBuffer[1024];

	do {
		if (!fgets(inputBuffer, 1024, stdin)) {
			printf(" Input failed for some reason");
			exit(1);
		}

		if (strcspn(inputBuffer, "\n") >= 2) {
			printf("\nToo many characters, retry: ");
			resultChar = '\0';
		} else if (strcspn(inputBuffer, "\n") == 0) {
			printf("\nNo input received, retry: ");
			resultChar = '\0';
		} else {
			memcpy(&resultChar, &inputBuffer[0], 1);
		}
	} while (resultChar == '\0');

	return resultChar;
}

char* ReadString() {
	bool validInput = false;
	char* inputStr = malloc(sizeof(char) * 50);
	char inputBuffer[1024];

	do {
		bool containsDigit = false;
		bool containsAlpha = false;
		bool containsWhitespace = false;

		scanf(" %s", inputBuffer);
		ClearInputBuffer();
		strcpy(inputStr, RemoveAllWhitespace(inputBuffer));

		for (int i = 0; inputStr[i] != '\0'; i++) {
			if (isalpha(inputStr[i])) {
				containsAlpha = true;
			}
			if (isdigit(inputStr[i])) {
				containsDigit = true;
			}
			if (IsWhitespace(inputStr[i])) {
				containsWhitespace = true;
			}
		}

		if (((containsDigit + containsAlpha + containsWhitespace) < 2) && !containsWhitespace) {
			validInput = true;
		} else {
			printf("\nInvalid input, try again: ");
		}
	} while (validInput == false);
	
	return inputStr;
}

char* EquationAsString(Equation eq) {
	char* buffer = calloc(50, sizeof(char));

	char operand1Buffer[12];
	char operand2Buffer[12];
	char resultBuffer[12];
	char operatorBuffer[2];

	operatorBuffer[0] = eq.operatorChar;
	operatorBuffer[1] = '\0';
	sprintf(operand1Buffer, "%.3f", eq.firstOperand);
	sprintf(operand2Buffer, "%.3f", eq.secondOperand);
	sprintf(resultBuffer, "%.4f", eq.result);

	strcat(buffer, operand1Buffer);
	strcat(buffer, spaceBuffer);
	strcat(buffer, operatorBuffer);
	strcat(buffer, spaceBuffer);
	strcat(buffer, operand2Buffer);
	strcat(buffer, equalsBuffer);
	strcat(buffer, resultBuffer);

	return buffer;
}

void PrintAllEquationsInList(EquationList* list) {
	struct EquationListNode* firstPtr = list->head;
	while (firstPtr != NULL) {
		char* strToPrint = EquationAsString(*(firstPtr->equation));
		printf(" %s\n", strToPrint);
		free(strToPrint);
		strToPrint = NULL;

		firstPtr = firstPtr->next;
	}
}

void InsertNewEquationIntoList(EquationList* list, struct EquationListNode* newEquationNode) {
	if (list->head == NULL) {
		list->head = newEquationNode;
		list->tail = newEquationNode;
	} else {
		newEquationNode->prev = list->tail;
		list->tail->next = newEquationNode;
		list->tail = newEquationNode;
	}
}

void DeleteList(EquationList* list) {
	struct EquationListNode* lastPtr = list->tail;
	while (lastPtr != NULL) {
		list->tail = list->tail->prev;
		free(lastPtr->equation);
		free(lastPtr);
		lastPtr = list->tail;
	}
}

bool IsStandardOperator(char operatorChar) {
	return (operatorChar == '+' || operatorChar == '-' || operatorChar == '*' || operatorChar == '/');
}

/*
bool IsUnaryOperator(char operatorChar) {
	return ;
}
*/

void NewEquation(EquationList* equationList) {
	float firstOperand;
	char operatorChar = '\0';
	float secondOperand;
	float result;

	char firstOperandBuffer[50];
	char secondOperandBuffer[50];
	char* firstOperandStr = malloc(sizeof(char) * 50);
	char* secondOperandStr = malloc(sizeof(char) * 50);

	bool errorEncountered = false;
/*
	printf("\nFirst operand: ");
	scanf(" %s", firstOperandBuffer);
	strcpy(firstOperandStr, RemoveAllWhiteSpace(firstOperandBuffer));
	ClearInputBuffer();
*/

	printf("\nFirst operand: ");
	firstOperandStr = ReadString();

	printf("\nOperator ((+)(-)(/)(*)): ");
	while (!IsStandardOperator(operatorChar)) {
		operatorChar = ReadChar();
		if (!IsStandardOperator(operatorChar)) {
			printf("\nInvalid operation, must be (+)(-)(/)(*)");
		}
	}

/*
	printf("\nSecond operand: ");
	scanf(" %s", secondOperandBuffer);
	strcpy(secondOperandStr, RemoveAllWhitespace(secondOperandBuffer));
	ClearInputBuffer();
*/

	printf("\nSecond operand: ");
	secondOperandStr = ReadString();

	if (strcmp(firstOperandStr, "ans") == 0) {
		if (equationList->tail == NULL) {
			printf("Invalid ans usage\n");
			free(firstOperandStr);
			free(secondOperandStr);
			return;
		}
		firstOperand = equationList->tail->equation->result;
	} else {
		if (!StringIsNumber(firstOperandStr)) {
			printf("\nUnknown symbol %s, cancelling operation", firstOperandStr);
			free(firstOperandStr);
			free(secondOperandStr);
			return;
		}

		firstOperand = atof(firstOperandStr);
	}

	if (strcmp(secondOperandStr, "ans") == 0) {
		if (equationList->tail == NULL) {
			printf("Invalid ans usage\n");
			free(firstOperandStr);
			free(secondOperandStr);
			return;
		}
		secondOperand = equationList->tail->equation->result;
	} else {
		if (!StringIsNumber(firstOperandStr)) {
			printf("\nUnknown symbol %s, cancelling operation", secondOperandStr);
			free(firstOperandStr);
			free(secondOperandStr);
			return;
		}
		secondOperand = atof(secondOperandStr);
	}

	free(firstOperandStr);
	free(secondOperandStr);
	switch (operatorChar) {
		case '+':
			result = firstOperand + secondOperand;
			break;
		case '-':
			result = firstOperand - secondOperand;
			break;
		case '*':
			result = firstOperand * secondOperand;
			break;
		case '/':
			if (secondOperand == 0.0) {
				errorEncountered = true;
				printf("divide by zero error!\n");
			} else {
				result = firstOperand / secondOperand;
			}
			break;
		default:
			printf("unable to compute\n");
			errorEncountered = true;
			break;
	}

	if (errorEncountered) {
		printf("failed to compute\n");
		return;
	} else {
		printf("\n%.3f", firstOperand);
		printf(" %c", operatorChar);
		printf(" %.3f", secondOperand);

		printf(" = %.4f", result);
	}

	Equation* newEquation = malloc(sizeof(Equation));
	newEquation->firstOperand = firstOperand;
	newEquation->secondOperand = secondOperand;
	newEquation->operatorChar = operatorChar;
	newEquation->result = result;

	struct EquationListNode* newEquationNode = NewEquationNode();
	newEquationNode->equation = newEquation;
	InsertNewEquationIntoList(equationList, newEquationNode);
}

void AssignNewVariable(Variable newVar) {
	char inputChar;
	char* inputVal = malloc(sizeof(char) * 50);
	printf("\nEnter new variable name (must be one letter): ");
	inputChar = ReadChar();
	
	printf("\nEnter value for variable: ");
	do {
		inputVal = ReadString();
		if (!StringIsNumber(inputVal)) {
			printf("\nInvalid input, try again: ");
		}
	} while (!StringIsNumber(inputVal));

	float actualVal = atof(inputVal);

	newVar.variableName = inputChar;
	newVar.value = actualVal;
}

int main() {
	char userInput;
	bool inApplication = true;
	EquationList* equationList = NewEquationList();
	Variable variableInMemory;

	printf("Welcome to my first program: a four-function calculator with equation memory!\n");
	printf("Standard inputs in this menu are: \n");
	printf("n - new calculation\n");
	printf("m - see all equations in memory\n");
	printf("v - declare new variable\n");
	printf("q - quit application\n");


	while (inApplication) {
		printf("\nEnter input here: ");
		userInput = ReadChar();

		switch (userInput) {
			case 'n':
				NewEquation(equationList);
				break;
			case 'm':
				PrintAllEquationsInList(equationList);
				break;
			case 'v':
				AssignNewVariable(variableInMemory);
				break;
			case 'q':
				inApplication = false;
				break;
			default:
				printf("\nFailed input! Valid inputs are (n, m, q)");
				break;
		}
	}

	DeleteList(equationList);
	return 0;
}
