#ifndef __ERRORTYPE_CPP__
#define __ERRORTYPE_CPP__

#include <ErrorType.h>

ErrorType::ErrorType(const std::string &name, const int n_universes, const Type type, const std::string &fill_samples) : m_name(name), m_Nuniverses(n_universes), m_type(type)
{
	m_fill_sample.push_back(fill_samples);
}

ErrorType::~ErrorType()
{
	m_fill_sample.clear();
}

#endif