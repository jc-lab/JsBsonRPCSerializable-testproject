#include <stdio.h>

#include <string>
#include <map>
#include <list>
#include <vector>

#include "JsBsonRPCSerializable/Serializable.h"
#include "JsBsonRPCSerializable/plugins/JSONObjectMapper.h"


class TestSubClassCommon : public JsBsonRPC::Serializable
{
public:
	TestSubClassCommon() : Serializable("x", 10) {}

	TestSubClassCommon(const char *name) : Serializable(name, 101)
	{
	}
};

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

class TestSubClassA : public TestSubClassCommon
{
public:
	JsBsonRPC::SType<int32_t> a;
	JsBsonRPC::SType<std::string> b;
	JsBsonRPC::SType<std::string> sa;

	TestSubClassA() : TestSubClassCommon("testA")
	{
		this->serializableMapMember("a", a);
		this->serializableMapMember("b", b);
		this->serializableMapMember("sa", sa);
	}
};

class TestSubClassACreateFactory : public JsBsonRPC::SerializableSmartpointerCreateFactory
{
public:
	JsCPPUtils::SmartPointer<JsBsonRPC::Serializable> create()
	{
		return new TestSubClassA();
	}
};

class TestSubClassB : public TestSubClassCommon
{
public:
	JsBsonRPC::SType<int32_t> a;
	JsBsonRPC::SType<std::string> b;
	JsBsonRPC::SType<std::string> sb;

	TestSubClassB() : TestSubClassCommon("testB")
	{
		this->serializableMapMember("a", a);
		this->serializableMapMember("b", b);
		this->serializableMapMember("sb", sb);
	}
};

class TestSubClassBCreateFactory : public JsBsonRPC::SerializableSmartpointerCreateFactory
{
public:
	JsCPPUtils::SmartPointer<JsBsonRPC::Serializable> create()
	{
		return new TestSubClassB();
	}
};

class DynamicCreateFactory : public JsBsonRPC::SerializableSmartpointerCreateFactory
{
public:
	JsCPPUtils::SmartPointer<JsBsonRPC::Serializable> create()
	{
		return NULL;
	}
	JsCPPUtils::SmartPointer<JsBsonRPC::Serializable> create(const std::string& name, int64_t serialVersionUID) override
	{
		if (name == "testA")
			return new TestSubClassA();
		if (name == "testB")
			return new TestSubClassB();
		return NULL;
	}
};

class TestClassA : public JsBsonRPC::Serializable
{
private:
	TestSubClassBCreateFactory testBFactory;
	DynamicCreateFactory dynamicFactory;

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
	JsBsonRPC::SType< std::list< JsCPPUtils::SmartPointer<TestSubClassB> > > sublist;
	JsBsonRPC::SType<std::map<std::string, TestSubClassB> > submap;

	JsBsonRPC::SType< JsCPPUtils::SmartPointer<TestSubClassCommon> > dynamic;

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
		this->serializableMapMember("sublist", sublist).setCreateFactory(&testBFactory);
		this->serializableMapMember("submap", submap);
		this->serializableMapMember("dynamic", dynamic).setCreateFactory(&dynamicFactory);
	}
};

class TestClassASmall : public JsBsonRPC::Serializable
{
private:
	TestSubClassBCreateFactory testBFactory;

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

	TestClassASmall() : Serializable("test", 101)
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
	}
};

int main()
{
	std::vector<unsigned char> payload;

	std::vector<char> test;

	TestClassA testA;
	TestClassA testB;
	TestClassA testC;
	TestClassASmall testASmall;

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

	testA.dynamic.ref() = new TestSubClassB();

	{
		JsCPPUtils::SmartPointer<TestSubClassB> a = new TestSubClassB();
		a->a.set(1);
		a->b.set("a");
		testA.sublist.ref().push_back(a);
	}

	{
		JsCPPUtils::SmartPointer<TestSubClassB> a = new TestSubClassB();
		a->a.set(2);
		a->b.set("b");
		testA.sublist.ref().push_back(a);
	}

	payload.clear();
	testA.serialize(payload);
	dump(payload);

	testB.deserialize(payload);

	testASmall.deserialize(payload);

	JsBsonRPC::JSONObjectMapper objectMapper;
	std::string jsondata = objectMapper.serialize(&testA);

	std::string name;
	int64_t sver;
	JsBsonRPC::Serializable::readMetadata(payload, 0, &name, &sver);

	printf("JSON : %s\n", jsondata.c_str());

	objectMapper.deserialize(&testC, jsondata);

	return 0;
}

