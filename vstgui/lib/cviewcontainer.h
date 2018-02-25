// This file is part of VSTGUI. It is subject to the license terms 
// in the LICENSE file found in the top-level directory of this
// distribution and at http://github.com/steinbergmedia/vstgui/LICENSE

#ifndef __cviewcontainer__
#define __cviewcontainer__

#include "vstguifwd.h"
#include "cview.h"
#include "cdrawdefs.h"
#if VSTGUI_TOUCH_EVENT_HANDLING
#include "itouchevent.h"
#endif
#include <list>
#include <memory>

namespace VSTGUI {

extern IdStringPtr kMsgLooseFocus;				///< Message of a view loosing focus (only CTextEdit and COptionMenu send this yet)

//-----------------------------------------------------------------------------
struct GetViewOptions
{
	enum {
		kNone					= 0,
		kDeep					= 1 << 0,
		kMouseEnabled			= 1 << 1,
		kIncludeViewContainer	= 1 << 2,
		kIncludeInvisible		= 1 << 3
	};

	explicit GetViewOptions (uint32_t options = kNone) : flags (options) {}

	GetViewOptions& deep (bool state = true) { setBit (flags, kDeep, state); return *this; }
	GetViewOptions& mouseEnabled (bool state = true) { setBit (flags, kMouseEnabled, state); return *this; }
	GetViewOptions& includeViewContainer (bool state = true) { setBit (flags, kIncludeViewContainer, state); return *this; }
	GetViewOptions& includeInvisible (bool state = true) { setBit (flags, kIncludeInvisible, state); return *this; }

	bool getDeep () const { return hasBit (flags, kDeep); }
	bool getMouseEnabled () const { return hasBit (flags, kMouseEnabled); }
	bool getIncludeViewContainer () const { return hasBit (flags, kIncludeViewContainer); }
	bool getIncludeInvisible () const { return hasBit (flags, kIncludeInvisible); }
private:
	uint32_t flags;
};

//-----------------------------------------------------------------------------
// CViewContainer Declaration
//! @brief Container Class of CView objects
/// @ingroup containerviews
//-----------------------------------------------------------------------------
class CViewContainer : public CView
{
public:
	using ViewList = std::list<SharedPointer<CView>>;

	explicit CViewContainer (const CRect& size);
	CViewContainer (const CViewContainer& viewContainer);

	//-----------------------------------------------------------------------------
	/// @name Sub View Methods
	//-----------------------------------------------------------------------------
	//@{
	bool addView (CView* pView, const CRect& mouseableArea, bool mouseEnabled = true);	///< add a child view
	virtual bool addView (CView* pView, CView* pBefore = nullptr);	///< add a child view before another view
	virtual bool removeView (CView* pView, bool withForget = true);	///< remove a child view
	virtual bool removeAll (bool withForget = true);	///< remove all child views
	bool isChild (CView* pView) const;	///< check if pView is a child view of this container
	virtual bool isChild (CView* pView, bool deep) const;	///< check if pView is a child view of this container
	virtual bool hasChildren () const;						///< check if container has child views
	virtual uint32_t getNbViews () const;			///< get the number of child views
	virtual CView* getView (uint32_t index) const;	///< get the child view at index
	virtual CView* getViewAt (const CPoint& where, const GetViewOptions& options = GetViewOptions ()) const;	///< get the view at point where
	virtual CViewContainer* getContainerAt (const CPoint& where, const GetViewOptions& options = GetViewOptions ().deep ()) const;		///< get the container at point where
	virtual bool getViewsAt (const CPoint& where, ViewList& views, const GetViewOptions& options = GetViewOptions ().deep ()) const;	///< get all views at point where, top->down
	virtual bool changeViewZOrder (CView* view, uint32_t newIndex);	///< change view z order position

	virtual bool hitTestSubViews (const CPoint& where, const CButtonState& buttons = -1);

	virtual void setAutosizingEnabled (bool state);					///< enable or disable autosizing subviews. Per default this is enabled.
	bool getAutosizingEnabled () const { return hasViewFlag (kAutosizeSubviews); }

	/** get child views of type ViewClass. ContainerClass must be a stdc++ container */
	template<class ViewClass, class ContainerClass>
	uint32_t getChildViewsOfType (ContainerClass& result, bool deep = false) const;

	/** execute proc for each child view */
	template<typename Proc>
	void forEachChild (Proc proc) const;

	//@}

	//-----------------------------------------------------------------------------
	/// @name Background Methods
	//-----------------------------------------------------------------------------
	//@{
	virtual void setBackgroundColor (const CColor& color);	///< set the background color (will only be drawn if this container is not set to transparent and does not have a background bitmap)
	virtual CColor getBackgroundColor () const;	///< get the background color
	virtual void setBackgroundOffset (const CPoint& p);	///< set the offset of the background bitmap
	virtual const CPoint& getBackgroundOffset () const;	///< get the offset of the background bitmap
	virtual void drawBackgroundRect (CDrawContext* pContext, const CRect& _updateRect);	///< draw the background
	
	virtual void setBackgroundColorDrawStyle (CDrawStyle style);
	CDrawStyle getBackgroundColorDrawStyle () const;
	//@}

	virtual bool advanceNextFocusView (CView* oldFocus, bool reverse = false);
	virtual bool invalidateDirtyViews ();
	virtual CRect getVisibleSize (const CRect& rect) const;

	void setTransform (const CGraphicsTransform& t);
	const CGraphicsTransform& getTransform () const;
	
	void registerViewContainerListener (IViewContainerListener* listener);
	void unregisterViewContainerListener (IViewContainerListener* listener);
	
	// CView
	void draw (CDrawContext* pContext) override;
	void drawRect (CDrawContext* pContext, const CRect& updateRect) override;
	CMouseEventResult onMouseDown (CPoint& where, const CButtonState& buttons) override;
	CMouseEventResult onMouseUp (CPoint& where, const CButtonState& buttons) override;
	CMouseEventResult onMouseMoved (CPoint& where, const CButtonState& buttons) override;
	CMouseEventResult onMouseCancel () override;
	bool onWheel (const CPoint& where, const float& distance, const CButtonState& buttons) override;
	bool onWheel (const CPoint& where, const CMouseWheelAxis& axis, const float& distance, const CButtonState& buttons) override;
	bool hitTest (const CPoint& where, const CButtonState& buttons = -1) override;
	CMessageResult notify (CBaseObject* sender, IdStringPtr message) override;

#if VSTGUI_TOUCH_EVENT_HANDLING
	virtual void onTouchEvent (ITouchEvent& event) override;
	virtual bool wantsMultiTouchEvents () const override { return true; }
	virtual void findSingleTouchEventTarget (ITouchEvent::Touch& event);
#endif

	bool onDrop (IDataPackage* drag, const CPoint& where) override;
	void onDragEnter (IDataPackage* drag, const CPoint& where) override;
	void onDragLeave (IDataPackage* drag, const CPoint& where) override;
	void onDragMove (IDataPackage* drag, const CPoint& where) override;

	void looseFocus () override;
	void takeFocus () override;

	bool isDirty () const override;

	void invalid () override;
	void invalidRect (const CRect& rect) override;
	
	void setViewSize (const CRect& rect, bool invalid = true) override;
	void parentSizeChanged () override;
	bool sizeToFit () override;

	bool removed (CView* parent) override;
	bool attached (CView* parent) override;
		
	CPoint& frameToLocal (CPoint& point) const override;
	CPoint& localToFrame (CPoint& point) const override;

	//-----------------------------------------------------------------------------
	using ChildViewConstIterator = ViewList::const_iterator;
	using ChildViewConstReverseIterator = ViewList::const_reverse_iterator;

	//-----------------------------------------------------------------------------
	template<bool reverse>
	class Iterator
	{
	public:
		explicit Iterator<reverse> (const CViewContainer* container) : children (container->getChildren ()) { if (reverse) riterator = children.rbegin (); else iterator = children.begin (); }
		Iterator<reverse> (const Iterator& vi) : children (vi.children), iterator (vi.iterator), riterator (vi.riterator) {}
		
		Iterator<reverse>& operator++ ()
		{
			if (reverse)
				++riterator;
			else
				++iterator;
			return *this;
		}
		
		Iterator<reverse> operator++ (int)
		{
			Iterator<reverse> old (*this);
			if (reverse)
				++riterator;
			else
				++iterator;
			return old;
		}
		
		Iterator<reverse>& operator-- ()
		{
			if (reverse)
				--riterator;
			else
				--iterator;
			return *this;
		}
		
		CView* operator* () const
		{
			if (reverse)
				return riterator != children.rend () ? *riterator : nullptr;
			return iterator != children.end () ? *iterator : nullptr;
		}
		
	protected:
		const ViewList& children;
		ChildViewConstIterator iterator;
		ChildViewConstReverseIterator riterator;
	};

	//-------------------------------------------
	CLASS_METHODS(CViewContainer, CView)

	#if DEBUG
	void dumpInfo () override;
	virtual void dumpHierarchy ();
	#endif

	CViewContainer* asViewContainer () final { return this; }
	const CViewContainer* asViewContainer () const final { return this; }

protected:
	enum {
		kAutosizeSubviews = 1 << (CView::kLastCViewFlag + 1)
	};
	
	~CViewContainer () noexcept override;
	void beforeDelete () override;
	
	virtual bool checkUpdateRect (CView* view, const CRect& rect);

	void setMouseDownView (CView* view);
	CView* getMouseDownView () const;
	
	const ViewList& getChildren () const;
private:
	struct Impl;
	std::unique_ptr<Impl> pImpl;
};

using ViewIterator = CViewContainer::Iterator<false>;
using ReverseViewIterator = CViewContainer::Iterator<true>;

//-----------------------------------------------------------------------------
template<class ViewClass, class ContainerClass>
inline uint32_t CViewContainer::getChildViewsOfType (ContainerClass& result, bool deep) const
{
	for (auto& child : getChildren ())
	{
		auto vObj = child.cast<ViewClass> ();
		if (vObj)
		{
			result.push_back (vObj);
		}
		if (deep)
		{
			if (auto container = child->asViewContainer ())
			{
				container->getChildViewsOfType<ViewClass, ContainerClass> (result);
			}
		}
	}
	return static_cast<uint32_t> (result.size ());
}

//-----------------------------------------------------------------------------
template <typename Proc>
inline void CViewContainer::forEachChild (Proc proc) const
{
	for (auto& child : getChildren ())
	{
		proc (child);
	}
}

} // namespace

#endif
