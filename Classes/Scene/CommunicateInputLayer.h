//
//  CommunicateInputLayer.h
//  dzpkV3.2
//
//  Created by apple on 14-9-12.
//
//

#ifndef dzpkV3_2_CommunicateInputLayer_h
#define dzpkV3_2_CommunicateInputLayer_h

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "../GlobalDef/Data/CommunicateData.h"
#include "CommunicateInputDelegate.h"
#include "../GlobalDef/GlobalDef.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

class CommunicateInputLayer : public LayerColor, public EditBoxDelegate{
public:
    CREATE_FUNC(CommunicateInputLayer);
    virtual bool init();
    
    void scheduleScrollListView(float dt);
    
    void btnCloseMenuCallback(Ref *ref);
    void btnSendMenuCallback(Ref *ref);
    
    void showRecvMsg(std::string msg);
    
    void setCommunicateInputDelegate(CommunicateInputDelegate* inputDelegate);
    
    void showText(std::string text);
    
    virtual bool onTextFieldAttachWithIME(TextFieldTTF * sender)
    {
        CC_UNUSED_PARAM(sender);
        return false;
    }
    
    /**
     * This method is called when an edit box gains focus after keyboard is shown.
     * @param editBox The edit box object that generated the event.
     * @js NA
     * @lua NA
     */
    virtual void editBoxEditingDidBegin(EditBox* editBox);
    
    /**
     * This method is called when an edit box loses focus after keyboard is hidden.
     * @param editBox The edit box object that generated the event.
     * @js NA
     * @lua NA
     */
    virtual void editBoxEditingDidEnd(EditBox* editBox);
    
    /**
     * This method is called when the edit box text was changed.
     * @param editBox The edit box object that generated the event.
     * @param text The new text.
     * @js NA
     * @lua NA
     */
    virtual void editBoxTextChanged(EditBox* editBox, const std::string& text);
    
    /**
     * This method is called when the return button was pressed or the outside area of keyboard was touched.
     * @param editBox The edit box object that generated the event.
     * @js NA
     * @lua NA
     */
    virtual void editBoxReturn(EditBox* editBox);
    
protected:
    Size visibleSize;
    
    ListView* listView;
    EditBox* editBox;
    
    CommunicateInputDelegate* inputDelegate;
};

#endif
