#pragma once
#include <iostream>
#include <string>
#include <memory>
#include "date.h"

using namespace std;

enum class Comparison {
	Less,
	LessOrEqual,
	Greater,
	GreaterOrEqual,
	Equal,
	NotEqual
};

enum class LogicalOperation {
	And,
	Or
};

class Node {
public:
	//Node() : type("") {}
	Node( const string& t ) : type(t) {}
	virtual bool Evaluate( const Date& date, const string& event ) =0;

private:
	string type;
};

class EmptyNode : public Node {
public:
	EmptyNode() : Node("Empty") {}
	bool Evaluate( const Date& date, const string& event ) { return true; }
};

class DateComparisonNode : public Node {
public:
	DateComparisonNode( const Comparison& cmp, const Date& d )
	: Node("DateComparison"), operation(cmp), dateValue(d)
	{}

	bool Evaluate( const Date& date, const string& event )
	{
		switch(operation) {
		case Comparison::Equal:
			return dateValue == date;
		case Comparison::NotEqual:
			return dateValue != date;
		case Comparison::Less:
			return dateValue > date;
		case Comparison::LessOrEqual:
			return dateValue >= date;
		case Comparison::Greater:
			return dateValue < date;
		case Comparison::GreaterOrEqual:
			return dateValue <= date;
		}
	}
private:
	Comparison operation;
	Date dateValue;
};

class EventComparisonNode : public Node {
public:
	EventComparisonNode( const Comparison& cmp, const string& s )
		: Node("EventComparison"), operation(cmp), eventValue(s)
	{}

	bool Evaluate( const Date& date, const string& event ) {
		switch(operation) {
		case Comparison::Equal:
			return eventValue == event;
		case Comparison::NotEqual:
			return eventValue != event;
		case Comparison::Less:
			return eventValue > event;
		case Comparison::LessOrEqual:
			return eventValue >= event;
		case Comparison::Greater:
			return eventValue < event;
		case Comparison::GreaterOrEqual:
			return eventValue <= event;
		default:
			cout << "Unknown comparison type\n";
			return false;
		}
		return false;
	}

private:
	Comparison operation;
	string eventValue;
};

class LogicalOperationNode : public Node {
public:
	LogicalOperationNode( LogicalOperation l, shared_ptr<Node> n_left, shared_ptr<Node> n_right )
		: Node("LogicalOperation"), lo(l), left(n_left), right(n_right)
	{}

	bool Evaluate( const Date& date, const string& event ) {
		if( lo == LogicalOperation::And ) {
			return left->Evaluate(date, event) && right->Evaluate(date, event);
		} else {
			return left->Evaluate(date, event) || right->Evaluate(date, event);
		}
	}

private:
	LogicalOperation lo;
	shared_ptr<Node> left;
	shared_ptr<Node> right;
};
