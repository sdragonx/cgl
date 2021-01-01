/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 expression.hpp

 2020-07-05 00:02:29

 ��ѧ����ʽ��

*/
#ifndef EXPRESSION_HPP_20200705000229
#define EXPRESSION_HPP_20200705000229

#include <cgl/public.h>

#include <vector>
#include <iostream>
#include <sstream>
#include <string>

namespace cgl{

//������������ȼ�
int priority(int n)
{
    switch(n&0xFF)
    {
    case '(':
    case ')':
        return 1;
    case '+':
    case '-':
        return 2;
    case '*':
    case '/':
    case '%':
        return 3;
    case '^':
        return 4;
    default:
        return n;
    }
}

//�ж���������ȼ�
bool compare(int op1, int op2)
{
    return priority(op1) >= priority(op2);
}

//������ֵ a op b
double compute(double a, double b, int op)
{
    switch(op){
    case '+':
        a += b;
        break;
    case '-':
        a -= b;
        break;
    case '*':
        a *= b;
        break;
    case '/':
        if(b == 0)
            cout<<"����=0"<<endl;
        else
            a /= b;
        break;
    case '%':
        a = fmod(a, b);
        break;
    case '^':
        a = pow(a, b);
        break;
    default:
        return 0;
    }
    return a;
}

struct subexp
{
    int type;
    string value;
};

class expression
{
public:
    std::vector<double>  values;    //��ֵ��ջ
    std::vector<int>     operators; //��������ջ
    double AX, BX;  //��ֵ�Ĵ���
    int OP;         //�������Ĵ���

    expression()
    {
    }

    //��������ʽ�����ر���ʽ��ջ
    int parse(vector<subexp>& stack, const string& str)
    {
        size_t begin = 0;
        size_t end = 0;

        char ch;
        subexp e;

        for( ; end < str.length(); ){
            ch = str[end];
            switch(ch){
            case ' ':
                if(begin == end){//�����ո�
                }
                else{
                    e.type = 0;//��ֵ����
                    e.value = str.substr(begin, end - begin);
                    stack.push_back(e);
                }
                begin = ++end;
                break;
            case '+':
            case '-':
            case '*':
            case '/':
            case '%':
            case '(':
            case ')':
                if(begin != end){
                    e.type = 0;
                    e.value = str.substr(begin, end - begin);
                    stack.push_back(e);
                 }
                e.type = ch;
                e.value.clear();
                stack.push_back(e);
                begin = ++end;
                break;
            default:
                ++end;
                break;
            }
        }

        //���ﲻ���Ƿ���
        if(begin != end){
            e.type = 0;
            e.value = str.substr(begin, end - begin);
            stack.push_back(e);
        }
        return 0;
    }

    //��������ı���ʽ��ջ
    double calc(const vector<subexp>& stack)
    {
        values.clear();
        operators.clear();

        AX = BX = 0;
        OP = 0;

        char ch;

        //������ջѹ��һ��0
        operators.push_back(0);

        for(size_t i=0; i<stack.size(); ++i){
            ch = stack[i].type;
            switch(ch){
            case 0://��ֵ
                values.push_back(to_double(stack[i].value));
                break;
            case '+':
            case '-':
            case '*':
            case '/':
            case '%':
                //���������ջ����Ϊ0���Ƚϲ��������ȼ�
                if(operators.back() && compare(operators.back(), stack[i].type)){
                    if (!calc()) {
                        return 0.0;
                    }
                }
                operators.push_back(ch);
                break;
            case '(':
                operators.push_back(ch);
                break;
            case ')':
                while(operators.back()!='('){
                    if (!calc()) {
                        return 0.0;
                    }
                }
                if(operators.empty()){
                    return 0.0;
                }
                operators.pop_back();
                break;
            default:
                break;

            }
        }

        //���������ж�ջ�е�����
        //˳��ִ��
        while(operators.back()){
            if (!calc()) {
                return 0.0;
            }
        }

        //����ֵջ��
        return values.back();
    }

private:
    double to_double(const string& str)
    {
        stringstream stm(str);
        double n;
        stm>>n;
        return n;
    }

    void push_value(int value)
    {
        values.push_back(value);
    }

    void push_op(int op)
    {
        operators.push_back(op);
    }

    //����һ�����ݵ��Ĵ���
    bool pop()
    {
        if (operators.empty()) {
            return false;
        }
        OP = operators.back();
        operators.pop_back();

        if (values.empty()) {
            return false;
        }
        BX = values.back();
        values.pop_back();
        if (values.empty()) {
            return false;
        }
        AX = values.back();
        values.pop_back();

        return true;
    }

    //�������
    bool calc()
    {
        //����һ����ֵ
        if (!pop()) {
            return false;
        }
        //����
        AX = compute(AX, BX, OP);
        //���ѹ����ֵջ
        values.push_back(AX);
        //�Ĵ�������
        AX = BX = OP = 0;
        return true;
    }
};

}//end namespace cgl

#endif //EXPRESSION_HPP_20200705000229