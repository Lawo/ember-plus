#include "EnumerationView.h"
#include "gadget/EnumParameter.h"
#include <QApplication>
#include <QComboBox>
#include <iostream>
class Parameter:public gadget::EnumParameter{public:int writes=0;Parameter():EnumParameter(nullptr,"enum",1){}void setIndexImpl(size_type,bool){++writes;}};
int main(int argc,char**argv){QApplication app(argc,argv);Parameter p;std::vector<std::string> a{"A","B","C"};p.assign(a.begin(),a.end());p.setIndex(2);EnumerationView view(nullptr,&p);auto box=view.findChild<QComboBox*>("valueComboBox");int failures=0;auto check=[&](bool ok,const char*n){std::cout<<n<<": "<<(ok?"PASS":"FAIL")<<std::endl;if(!ok)++failures;};int before=p.writes;a={"X","Y","Z"};p.assign(a.begin(),a.end());check(p.index()==2&&box->currentIndex()==2&&p.writes==before,"label rebuild preserves selected value without writes");box->setCurrentIndex(1);check(p.index()==1,"subsequent user selection still connected");box->blockSignals(true);a={"L","M","N"};p.assign(a.begin(),a.end());check(box->signalsBlocked()&&p.index()==1&&box->currentIndex()==1,"previous blocked state preserved");box->blockSignals(false);a={"only"};p.assign(a.begin(),a.end());check(p.index()==0&&box->currentIndex()==0,"shrinking enumeration keeps model fallback");return failures?1:0;}
