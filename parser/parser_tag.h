#include<string>
#include<vector>
#include<algorithm>
using namespace std;

namespace pl{
	
	typedef char Ttarget;
	class match_result{
	public:
		match_result(Ttarget* b, Ttarget* e){
			begin=b;
			end=e;
		}
		bool operator ==(const match_result& o){
			return o.begin==begin && o.end==end;
		}
		Ttarget* begin;
		Ttarget* end;
		vector<match_result> items;
		string name;
	};
	class tag{
	public:
		virtual~tag(){};
		virtual vector<match_result> match(Ttarget* ptr)=0;
	};

	class tag_any: public tag{
	public:
		Ttarget obj;
		tag_any(Ttarget _obj): obj(_obj){}
		vector<match_result> match(Ttarget* ptr){
			vector<match_result> ret;
			if(obj==*ptr)
				ret.push_back(match_result(ptr, ptr+1));
			return move(ret);
		}
	};

	class tag_named: public tag{
	public:
		string name;
		tag* tag_a;
		tag_named(string _name, tag* _a): name(_name), tag_a(_a){}
		vector<match_result> match(Ttarget* ptr){
			vector<match_result> ret;
			vector<match_result> tmp_a= tag_a->match(ptr);
			for(auto iter=tmp_a.begin(); iter!=tmp_a.end(); ++iter){
				match_result temp(*iter);
				temp.name= name;
				temp.items.push_back(*iter);
				ret.push_back(temp);
			}
			return move(ret);
		}
	};

	class tag_or: public tag{
	public:
		tag* tag_a;
		tag* tag_b;
		tag_or(tag* _a, tag* _b): tag_a(_a), tag_b(_b){}
		~tag_or(){
			delete tag_a;
			delete tag_b;
		}
		vector<match_result> match(Ttarget* ptr){
			vector<match_result> ret;
			vector<match_result> tmp_a= tag_a->match(ptr);
			for(auto iter=tmp_a.begin(); iter!=tmp_a.end(); ++iter){
				match_result temp(*iter);
				temp.items.push_back(*iter);
				ret.push_back(temp);
			}
			vector<match_result> tmp_b= tag_b->match(ptr);
			for(auto iter=tmp_b.begin(); iter!=tmp_b.end(); ++iter){
				match_result temp(*iter);
				temp.items.push_back(*iter);
				ret.push_back(temp);
			}
			return move(ret);
		}
	};
		
	class tag_and: public tag{
	public:
		tag* tag_a;
		tag* tag_b;
		tag_and(tag* _a, tag* _b): tag_a(_a), tag_b(_b){}
		vector<match_result> match(Ttarget* ptr){
			vector<match_result> ret;
			vector<match_result> tmp_a= tag_a->match(ptr);
			vector<match_result> tmp_b= tag_b->match(ptr);
			for(auto iter=tmp_a.begin(); iter!=tmp_a.end(); ++iter){
				auto find_res= find(tmp_b.begin(),tmp_b.end(), *iter);
				if(find_res!= tmp_b.end() && find_res->end== iter->end){
					match_result temp(*iter);
					temp.items.push_back(*iter);
					temp.items.push_back(*find_res);
					ret.push_back(temp);
				}
			}
			return move(ret);
		}
	};

	class tag_link: public tag{
	public:
		tag* tag_a;
		tag* tag_b;
		tag_link(tag* _a, tag* _b): tag_a(_a), tag_b(_b){}
		vector<match_result> match(Ttarget* ptr){
			vector<match_result> ret;
			vector<match_result> tmp_a= tag_a->match(ptr);
			for(auto iter=tmp_a.begin(); iter!=tmp_a.end(); ++iter){
				vector<match_result> tmp_b= tag_b->match(iter->end);
				for(auto iter2=tmp_b.begin(); iter2!=tmp_b.end(); ++iter2){
					//{iter iter2}
					match_result temp(iter->begin, iter2->end);
					temp.items.push_back(*iter);
					temp.items.push_back(*iter2);
					ret.push_back(temp);
				}
			}
			return move(ret);
		}
	};


	bool _unittest_tag(){
		char dst[]= "banana";
		tag_any find_a('b');
		auto find_a_res= find_a.match(dst);
		tag_link find_bana(new tag_any('b'), new tag_link(new tag_any('a'), new tag_link(new tag_any('n'), new tag_any('a')))); 
		auto find_bana_res= find_bana.match(dst);
		tag_or find_a_or_b(new tag_any('a'), new tag_any('b'));
		auto find_a_or_b_res= find_a_or_b.match(dst);
		return true;
	}
}