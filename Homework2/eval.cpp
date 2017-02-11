//#include <iostream>
//#include <string>
//#include <stack>
//#include <cassert>
//
//using namespace std;
//
//bool isOperator(char ch);
//void removeSpaces(string& infix);
//bool checkInfix(string& infix);
//int precedence(char ch);
//void convert(string& infix, string& postfix);
//
//int evaluate(string infix, const bool values[], string& postfix, bool& result)
//{
//    //---------------------Pre Work For Evaluate-----------------------
//    removeSpaces(infix);
//    if (!checkInfix(infix))
//        return 1;
//    convert(infix, postfix);
//    //------------------------Start Evaluating-------------------------
//    stack<bool> operandStack;
//    for (unsigned int i = 0; i < postfix.size(); i++)
//    {
//        char ch = postfix[i];
//        bool operand1, operand2;
//        if (ch == '1' || ch == '2' || ch == '3' || ch == '4'
//            || ch == '5' || ch == '6' || ch == '7' || ch == '8'
//            || ch == '9' || ch == '0')
//            operandStack.push(values[ch-48]);
//        else
//        {
//            operand2 = operandStack.top();
//            operandStack.pop();
//            operand1 = operandStack.top();
//            operandStack.pop();
//            if (ch == '&')
//                operandStack.push(operand1 && operand2);
//            if (ch == '|')
//                operandStack.push(operand1 || operand2);
//            if (ch == '!')
//            {
//                operandStack.push(operand1);
//                operandStack.push(!operand2);
//            }
//        }
//    }
//    result = operandStack.top();
//    return 0;
//}
//
//bool isOperator(char ch)
//{
//    if (ch == '!' || ch == '&' || ch == '|' || ch == '(' || ch == ')')
//        return true;
//    else
//        return false;
//}
//
//void removeSpaces(string& infix)
//{
//    if (infix != "")
//        for(unsigned int i = 0; i < infix.size(); i++)
//            if(infix[i] == ' ')
//            {
//                infix.erase(i, 1);
//                i--;
//            }
//}
//
//bool checkInfix(string& infix)
//{
//    unsigned long last = infix.size() - 1;
//    int count1 = 0, count2 = 0;
//    if (infix != "")
//    {
//        if (infix[last] == '!' || infix[last] == '&' ||
//            infix[last] == '|' || infix[last] == '(')
//            return false;
//        
//        for(unsigned int i = 0; i < infix.size(); i++)
//        {
//            if (infix[i] != '0' && infix[i] != '1' && infix[i] != '2'
//                && infix[i] != '3' && infix[i] != '4' && infix[i] != '5'
//                && infix[i] != '6' && infix[i] != '7' && infix[i] != '8'
//                && infix[i] != '9' && infix[i] != '!' && infix[i] != '&'
//                && infix[i] != '|' && infix[i] != '(' && infix[i] != ')')
//                return false;
//            if(infix[i] == '(')
//                count1++;
//            if(infix[i] == ')')
//                count2++;
//        }
//        
//        if (count1 != count2)
//            return false;
//        
//        for (unsigned int i = 0; i < infix.size() - 1; i++)
//        {
//			if (!isOperator(infix[i]) && !isOperator(infix[i + 1]))
//				return false;
//			else if (isOperator(infix[i]) && isOperator(infix[i + 1])
//				&& infix[i + 1] != '!' && infix[i + 1] != '(' && (infix[i] != ')' && infix[i + 1] != ')'))
//				return false;
//            else if (!isOperator(infix[i]) && infix[i+1] == '(')
//                return false;
//            else if (infix[i] == '(' && infix[i+1] == ')')
//                return false;
//        }
//        return true;
//    }
//    return false;
//}
//
//int precedence(char ch)
//{
//    switch (ch)
//    {
//        case '!':
//            return 2;
//			break;
//        case '&':
//			return 1;
//			break;
//        case '|':
//            return 0;
//			break;
//    }
//    return -1;
//}
//
//void convert(string& infix, string& postfix)
//{
//    removeSpaces(infix);
//    postfix = "";
//    stack<char> operatorStack;
//    char ch;
//    for (unsigned int i = 0; i < infix.size(); i++)
//    {
//        ch = infix[i];
//        switch(ch)
//        {
//            case '1':
//            case '2':
//            case '3':
//            case '4':
//            case '5':
//            case '6':
//            case '7':
//            case '8':
//            case '9':
//            case '0':
//                postfix += ch;
//                break;
//            case '(':
//                operatorStack.push(ch);
//                break;
//            case ')':
//                while(operatorStack.top() != '(')
//                {
//                    postfix += operatorStack.top();
//                    operatorStack.pop();
//                }
//                operatorStack.pop();
//                break;
//            case '&':
//            case '|':
//            case '!':
//                while (!operatorStack.empty() && operatorStack.top() != '('
//                       && precedence(ch) <= precedence(operatorStack.top()))
//                       {
//                           postfix += operatorStack.top();
//                           operatorStack.pop();
//                       }
//                operatorStack.push(ch);
//                break;
//        }
//    }
//    while (!operatorStack.empty())
//    {
//        postfix += operatorStack.top();
//        operatorStack.pop();
//    }
//}
//
//int main()
//{
//	bool ba[10] = {
//		//  0      1      2      3      4      5      6      7      8      9
//		true,  true,  true,  false, false, false, true,  false, true,  false
//	};
//	string pf;
//	bool answer;
//	assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" &&  answer);
//	assert(evaluate("8|", ba, pf, answer) == 1);
//	assert(evaluate("4 5", ba, pf, answer) == 1);
//	assert(evaluate("01", ba, pf, answer) == 1);
//	assert(evaluate("()", ba, pf, answer) == 1);
//	assert(evaluate("2(9|8)", ba, pf, answer) == 1);
//	assert(evaluate("2(&8)", ba, pf, answer) == 1);
//	assert(evaluate("(6&(7|7)", ba, pf, answer) == 1);
//	assert(evaluate("", ba, pf, answer) == 1);
//	assert(evaluate("4  |  !3 & (0&3) ", ba, pf, answer) == 0
//		&& pf == "43!03&&|" && !answer);
//	assert(evaluate(" 9  ", ba, pf, answer) == 0 && pf == "9" && !answer);
//	ba[2] = false;
//	ba[9] = true;
//	assert(evaluate("((9))", ba, pf, answer) == 0 && pf == "9"  &&  answer);
//	assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" && !answer);
//	cout << "Passed all tests" << endl;
//	cin.get();
//}
