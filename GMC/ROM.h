#pragma once
#include <string>
#include <vector>
#include <optional>


class ROM
{
public:

	using container_type = std::vector<unsigned char>;
	using value_type = container_type::value_type;
	using size_type = container_type::size_type;

	static const unsigned BankSize = 16384;	//	8k banks

	virtual ~ROM() = default;

	virtual bool Load(const std::string& filename);
	virtual void Reset();

	virtual void SetBank(unsigned char blockId);
	virtual unsigned char GetBank() const;
	virtual value_type Read(size_type offset) const;


private:

	unsigned char	m_Bank = 0;
	size_type		m_BankOffset = 0;
	container_type	m_Data;
};