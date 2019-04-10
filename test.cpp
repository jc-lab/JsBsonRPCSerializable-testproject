#include <stdio.h>

#include <string>
#include <map>
#include <list>
#include <vector>

#include "JsBsonRPCSerializable/Serializable.h"
#include "JsBsonRPCSerializable/plugins/JSONObjectMapper.h"

void dump(std::vector<unsigned char> &buffer)
{
	size_t i = 0;
	size_t len = buffer.size();
	for (i = 0; i < len; i++)
	{
		printf("%02x ", buffer[i]);
	}
	printf("\n");
}

class TestSubClassB : public JsBsonRPC::Serializable
{
public:
	JsBsonRPC::SType<int32_t> a;
	JsBsonRPC::SType<std::string> b;

	TestSubClassB() : Serializable("test2", 101)
	{
		this->serializableMapMember("a", a);
		this->serializableMapMember("b", b);
	}
};

class TestClassA : public JsBsonRPC::Serializable
{
public:
	JsBsonRPC::SType<int32_t> a;
	JsBsonRPC::SType<int64_t> b;
	JsBsonRPC::SType<double> d;
	JsBsonRPC::SType<std::string> e;
	JsBsonRPC::SType<std::vector<char> > xa; // binary
	JsBsonRPC::SType<std::list<std::string> > xb;
	JsBsonRPC::SType<std::list< std::vector<char> > > xc;
	JsBsonRPC::SType<double> xd;
	JsBsonRPC::SType<std::map<std::string, std::string> > xf;

	JsBsonRPC::SType<TestSubClassB> sub;
	JsBsonRPC::SType<std::map<std::string, TestSubClassB> > submap;

	TestClassA() : Serializable("test", 101)
	{
		this->serializableMapMember("a", a);
		this->serializableMapMember("b", b);
		this->serializableMapMember("d", d);
		this->serializableMapMember("e", e);
		this->serializableMapMember("xa", xa);
		this->serializableMapMember("xb", xb);
		this->serializableMapMember("xc", xc);
		this->serializableMapMember("xd", xd);
		this->serializableMapMember("xf", xf);
		this->serializableMapMember("sub", sub);
		this->serializableMapMember("submap", submap);
	}
};

int main()
{
	std::vector<unsigned char> payload;

	std::vector<char> test;

	TestClassA testA;
	TestClassA testB;
	TestClassA testC;

	testA.a.set(0xff);
	testA.b.set(0x1000000000000002);
	testA.d.set(3.14);
	testA.e.set("aaaa");
	testA.xd.set(1.1);

	testA.xa.ref().push_back('1');
	testA.xa.ref().push_back('2');
	testA.xa.ref().push_back('3');
	testA.xa.ref().push_back('4');
	testA.xb.ref().push_back("hello");
	testA.xb.ref().push_back("world");

	test.clear();
	test.push_back('1');
	test.push_back('1');
	test.push_back('1');
	testA.xc.ref().push_back(test);
	test.clear();
	test.push_back('2');
	test.push_back('2');
	test.push_back('2');
	testA.xc.ref().push_back(test);

	testA.xf.ref()["aaaa"] = "aaaa";
	testA.xf.ref()["bbbb"] = "bbbb";
	testA.xf.ref()["cccc"] = "c";
	testA.xf.ref()["ee"] = "ad";
	testA.xf.ref()["dd"] = "aae";

	testA.sub.ref().a.set(10);
	testA.sub.ref().b.set("SUB TEXT!");

	testA.submap.ref()["a"].a.set(10);
	testA.submap.ref()["a"].b.set("20");

	testA.submap.ref()["b"].a.set(10);
	testA.submap.ref()["b"].b.set("40");

	payload.clear();
	testA.serialize(payload);
	dump(payload);

	testB.deserialize(payload);

	JsBsonRPC::JSONObjectMapper objectMapper;
	std::string jsondata = objectMapper.serialize(&testA);

	objectMapper.deserialize(&testC, jsondata);

	printf("JSON : %s\n", jsondata.c_str());

	return 0;
}

