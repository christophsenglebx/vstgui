// This file is part of VSTGUI. It is subject to the license terms
// in the LICENSE file found in the top-level directory of this
// distribution and at http://github.com/steinbergmedia/vstgui/LICENSE

#include "switchcreators.h"
#include "multibitmapcontrolcreator.h"

#include "../../lib/controls/cswitch.h"
#include "../detail/uiviewcreatorattributes.h"
#include "../uiattributes.h"
#include "../uiviewcreator.h"
#include "../uiviewfactory.h"

//------------------------------------------------------------------------
namespace VSTGUI {
namespace UIViewCreator {

//------------------------------------------------------------------------
bool CSwitchBaseCreator::getAttributeNames (std::list<std::string>& attributeNames) const
{
	attributeNames.emplace_back (kAttrInverseBitmap);
	return IMultiBitmapControlCreator::getAttributeNames (attributeNames);
}

//------------------------------------------------------------------------
auto CSwitchBaseCreator::getAttributeType (const std::string& attributeName) const -> AttrType
{
	if (attributeName == kAttrInverseBitmap)
		return kBooleanType;
	return IMultiBitmapControlCreator::getAttributeType (attributeName);
}

//------------------------------------------------------------------------
bool CSwitchBaseCreator::apply (CView* view, const UIAttributes& attributes,
                                const IUIDescription* description) const
{
	auto control = dynamic_cast<CSwitchBase*> (view);
	if (!control)
		return false;

	bool b;
	if (attributes.getBooleanAttribute (kAttrInverseBitmap, b))
	{
		control->setInverseBitmap (b);
	}

	return IMultiBitmapControlCreator::apply (view, attributes, description);
}

//------------------------------------------------------------------------
bool CSwitchBaseCreator::getAttributeValue (CView* view, const std::string& attributeName,
                                            std::string& stringValue,
                                            const IUIDescription* desc) const
{
	auto control = dynamic_cast<CSwitchBase*> (view);
	if (!control)
		return false;

	if (attributeName == kAttrInverseBitmap)
	{
		stringValue = control->getInverseBitmap () ? strTrue : strFalse;
		return true;
	}
	return IMultiBitmapControlCreator::getAttributeValue (view, attributeName, stringValue, desc);
}

//------------------------------------------------------------------------
CVerticalSwitchCreator::CVerticalSwitchCreator ()
{
	UIViewFactory::registerViewCreator (*this);
}

//------------------------------------------------------------------------
IdStringPtr CVerticalSwitchCreator::getViewName () const
{
	return kCVerticalSwitch;
}

//------------------------------------------------------------------------
IdStringPtr CVerticalSwitchCreator::getBaseViewName () const
{
	return kCControl;
}

//------------------------------------------------------------------------
UTF8StringPtr CVerticalSwitchCreator::getDisplayName () const
{
	return "Vertical Switch";
}

//------------------------------------------------------------------------
CView* CVerticalSwitchCreator::create (const UIAttributes& attributes,
                                       const IUIDescription* description) const
{
	return new CVerticalSwitch (CRect (0, 0, 0, 0), nullptr, -1, nullptr);
}

//------------------------------------------------------------------------
CHorizontalSwitchCreator::CHorizontalSwitchCreator ()
{
	UIViewFactory::registerViewCreator (*this);
}

//------------------------------------------------------------------------
IdStringPtr CHorizontalSwitchCreator::getViewName () const
{
	return kCHorizontalSwitch;
}

//------------------------------------------------------------------------
IdStringPtr CHorizontalSwitchCreator::getBaseViewName () const
{
	return kCControl;
}

//------------------------------------------------------------------------
UTF8StringPtr CHorizontalSwitchCreator::getDisplayName () const
{
	return "Horizontal Switch";
}

//------------------------------------------------------------------------
CView* CHorizontalSwitchCreator::create (const UIAttributes& attributes,
                                         const IUIDescription* description) const
{
	return new CHorizontalSwitch (CRect (0, 0, 0, 0), nullptr, -1, nullptr);
}

//------------------------------------------------------------------------
} // UIViewCreator
} // VSTGUI
