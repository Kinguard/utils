#ifndef NETSERVICES_H
#define NETSERVICES_H

#include <map>
#include <string>

/**
 * Constants and mappings for most common net service ports
 *
 */
namespace Utils
{
namespace Net {
namespace Service
{

constexpr unsigned int Echo			= 7;
constexpr unsigned int Ftp			= 21;
constexpr unsigned int SSH			= 22;
constexpr unsigned int Telnet		= 23;
constexpr unsigned int SMTP			= 25;
constexpr unsigned int TFTP			= 69;
constexpr unsigned int HTTP			= 80;
constexpr unsigned int POP3			= 110;
constexpr unsigned int NTP			= 123;
constexpr unsigned int IMAP2		= 143;
constexpr unsigned int LDAP			= 389;
constexpr unsigned int HTTPS		= 443;
constexpr unsigned int Syslog		= 514;
constexpr unsigned int Submission	= 587;
constexpr unsigned int LDAPS		= 636;
constexpr unsigned int RSync		= 873;
constexpr unsigned int FTPS			= 990;
constexpr unsigned int IMAPS		= 993;
constexpr unsigned int POP3S		= 995;
constexpr unsigned int OpenVPN		= 1194;
constexpr unsigned int ALT_SMTP		= 2525; // KGP/OP alternate smtp port for forwarding

const std::map<unsigned int,const char*> toString =
{
	{Net::Service::Echo				, "Echo"},
	{Net::Service::Ftp				, "Ftp"},
	{Net::Service::SSH				, "SSH"},
	{Net::Service::Telnet			, "Telnet"},
	{Net::Service::SMTP				, "SMTP"},
	{Net::Service::TFTP				, "TFTP"},
	{Net::Service::HTTP				, "HTTP"},
	{Net::Service::POP3				, "POP3"},
	{Net::Service::NTP				, "NTP"},
	{Net::Service::IMAP2			, "IMAP2"},
	{Net::Service::LDAP				, "LDAP"},
	{Net::Service::HTTPS			, "HTTPS"},
	{Net::Service::Syslog			, "Syslog"},
	{Net::Service::Submission		, "Submission"},
	{Net::Service::LDAPS			, "LDAPS"},
	{Net::Service::RSync			, "RSync"},
	{Net::Service::FTPS				, "FTPS"},
	{Net::Service::IMAPS			, "IMAPS"},
	{Net::Service::POP3S			, "POP3S"},
	{Net::Service::OpenVPN			, "OpenVPN"},
	{Net::Service::ALT_SMTP			, "ALT_SMTP"},
};

} //NS Service
} // NS Net
} // NS Utils

#endif // NETSERVICES_H
