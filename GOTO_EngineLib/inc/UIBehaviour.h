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

        // UI ���� ����������Ŭ
        virtual void OnRectTransformDimensionsChange() {}
        virtual void OnCanvasHierarchyChanged() {}
    };
}