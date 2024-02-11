#include <Xeph2D.h>

namespace Xeph2D
{
    class CoolScript : public Component
    {
        XEPH2D_REG_COMP(0x0867932b);

    private:
	    // Used to load and save variables in the Editor
        void Serializables() override;
		
		// Called when the Component is created
        void Start() override;
		
		//Called every frame
        void Update() override;
    };
}
