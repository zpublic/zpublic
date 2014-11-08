#include <Sockets/Json.h>
#include <iostream>


void test(const std::string& str)
{
	Json js = Json::Parse(str);
	if (js.ToString() != str)
	{
		std::cout << "Original: " << str << "\n";
		std::cout << "ToString: " << js.ToString() << "\n";
	}
	else
	{
		std::cout << "OK: " << str << "\n";
	}
}


int main(int argc, char *argv[])
{
	try
	{
		Json js;

		test("\"String\"");
		test( "1234567" );
		test( "12345.7" );
		test( "[1,2,3,4,\"5\"]" );

		std::string str("Test\\ string\\ end\\\nAnders was here\n\"Quoted string inside string\"\n");
		std::cout << " --------------- original\n";
		std::cout << str << std::endl;

		js.encode(str);
		std::cout << " --------------- encoded\n";
		std::cout << str << std::endl;

		js.decode(str);
		std::cout << " --------------- decoded\n";
		std::cout << str << std::endl;

		js["String"] = str;
		str = js.ToString();
		test(str);

		std::cout << " --------------- from ToString()\n";
		std::cout << str << std::endl;

		Json js2 = Json::Parse(str);
		std::cout << " ---- parsed result from ToString()\n";
		std::cout << js2.ToString() << std::endl;

		std::cout << (std::string)js2["String"] << std::endl;

		std::cout << " ---- test parse\n";
		str = "{\"no-value\":null}";
		Json js3 = Json::Parse(str);
		std::cout << js3.ToString() << std::endl;

		std::cout << " --------------- request\n";
		Json req;
		req["method"] = "test";
		Json params;
		params["value1"] = 1L;
		params["value2"] = "2";
		req["params"] = params;
		std::cout << req.ToString() << std::endl;

		test(req.ToString());
	}
	catch (const Exception& e)
	{
		std::cerr << e.ToString() << std::endl;
	}
}


