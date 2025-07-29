#pragma once
#include "ScriptBehaviour.h"

namespace GOTOEngine
{
    class Canvas;
    class RectTransform;

    class UIBehaviour : public ScriptBehaviour
    {
    protected:
        UIBehaviour();
        virtual ~UIBehaviour();

        // UI 전용 라이프사이클
        virtual void OnRectTransformDimensionsChange() {}
        virtual void OnCanvasHierarchyChanged() {}
    };
}