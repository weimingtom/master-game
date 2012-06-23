#if defined __MY_UNIX__
#include "Component.hpp"
#endif
#if defined __MY_WIN32__
#include "../Component.hpp"
#endif


class CompVisual : public Component {
	//Component interface
public:
	virtual const comp_id_type& familyID() const { return mFamilyID; }

	//CompVisual interface
	//virtual void Draw() const = 0;
private:
	static comp_id_type mFamilyID;
};
