#ifndef __ERRORTYPE_H__
#define __ERRORTYPE_H__

#include <string>
#include <vector>

class ErrorType{
public:
	enum Type { kLateral = 0, kLateralCV, kVertical, kVerticalCV,  kUnCorError, kUnCorErrorCV, kNotSet };
	ErrorType() : m_name(""), m_Nuniverses(1), m_type(kNotSet) { m_fill_samples.clear(); }
	ErrorType(const std::string &name, const int n_universes, const Type type, const std::string &fill_samples = "");
	~ErrorType();

	std::string GetName() const { return m_name; }
	int GetNUniverses() const { return m_Nuniverses; }
	Type GetType() const { return m_type; }

private:
	std::string m_name;
	int m_Nuniverses;
	Type m_type;
	std::vector<std::string> m_fill_samples;
};

bool operator==(const ErrorType& lhs, const ErrorType& rhs)
{
    return ( lhs.GetName().compare( rhs.GetName() ) == 0 );
}

#endif