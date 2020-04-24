#ifndef H_RAMP_VISUALS_PROPERTY
#define H_RAMP_VISUALS_PROPERTY

class RampVisualsProperty: public BasePropertyDialog
{

public:
    RampVisualsProperty(VectorProtected<ISelect> *pvsel);
    virtual ~RampVisualsProperty()
    {
    }

    void UpdateVisuals(const int dispid=-1);
    void UpdateProperties(const int dispid);

protected:
    virtual BOOL OnInitDialog();

private:
    CComboBox m_typeCombo;
    CComboBox m_materialCombo;
    CComboBox m_imageCombo;
    CComboBox m_modeCombo;
    EditBox   m_depthBiasEdit;
    EditBox   m_topHeightEdit;
    EditBox   m_bottomHeightEdit;
    EditBox   m_topWidthEdit;
    EditBox   m_bottomWdthEdit;
    EditBox   m_leftWallEdit;
    EditBox   m_rightWallEdit;
    EditBox   m_diameterEdit;
    EditBox   m_distanceXEdit;
    EditBox   m_distanceYEdit;
    vector<string> m_typeList;
    vector<string> m_imageModeList;
};

#endif
