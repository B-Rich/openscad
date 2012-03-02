#ifndef VALUE_H_
#define VALUE_H_

#include <vector>
#include <string>
#include <Eigen/Core>
#include <Eigen/LU>
#include "polyset.h"

class QuotedString : public std::string
{
public:
	QuotedString() : std::string() {}
	QuotedString(const std::string &s) : std::string(s) {}
};
std::ostream &operator<<(std::ostream &stream, const QuotedString &s);

class Filename : public QuotedString
{
public:
	Filename() : QuotedString() {}
	Filename(const std::string &f) : QuotedString(f) {}
};
std::ostream &operator<<(std::ostream &stream, const Filename &filename);

class Value
{
public:
	enum type_e {
          UNDEFINED,
          BOOL,
          NUMBER,
          RANGE,
          VECTOR,
          STRING,
          MATRIX,
        POLYSET
	};

	enum type_e type;

	bool b;
	double num;
	std::vector<Value*> vec;
	double range_begin;
	double range_step;
	double range_end;
        std::string text;
        Eigen::MatrixXd matrix;
        PolySet *poly;

	Value();
	~Value();

	Value(bool v);
	Value(double v);
        Value(int v);
        Value(const std::string &t);
        Value(const Eigen::MatrixXd &m);
        // Value(const PolySet &ps);

	Value(const Value &v);
	Value& operator = (const Value &v);

	Value operator ! () const;
	Value operator && (const Value &v) const;
	Value operator || (const Value &v) const;

	Value operator + (const Value &v) const;
	Value operator - (const Value &v) const;
	Value operator * (const Value &v) const;
	Value operator / (const Value &v) const;
	Value operator % (const Value &v) const;

	Value operator < (const Value &v) const;
	Value operator <= (const Value &v) const;
	Value operator == (const Value &v) const;
	Value operator != (const Value &v) const;
	Value operator >= (const Value &v) const;
	Value operator > (const Value &v) const;

	Value inv() const;

	bool getnum(double &v) const;
	bool getv2(double &x, double &y) const;
	bool getv3(double &x, double &y, double &z, double defaultval = 0.0) const;

	std::string toString() const;

	bool toBool() const;

	void append(Value *val);

private:
	void reset_undef();
};

std::ostream &operator<<(std::ostream &stream, const Value &value);

#endif
