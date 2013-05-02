#include<vector>
using namespace std;
#include"parser_tag.h"

namespace pl {

	typedef char Ttarget;
	
	class match_result{
	public:
		match_result(Ttarget* b, Ttarget* e){
			begin=b;
			end=e;
		}
		Ttarget* begin;
		Ttarget* end;
	};
}