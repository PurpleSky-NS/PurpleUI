#include "EditWindow.h"
/*Oncreate调用，创建这个窗口*/
void EditWindow::CreateEditor(int x, int y, const std::vector<EditDataMsg>& msgs)
{
	int maxW = 0;
	for (auto i : msgs)
	{
		int tipsLineSize = i.tips.size() + 5;//frame:4+'#'
		int dataLineSize = i.name.size() + i.resualt.size() + 14;//" : "+mark+frame+6byte空白
		if (tipsLineSize > maxW)
			maxW = tipsLineSize;
		if (dataLineSize > maxW)
			maxW = dataLineSize;
	}
	m_view.SetViewRect(m_srcView, { x,y,maxW,((int)msgs.size() + 1) * 3 });
	m_msgs = msgs;
	Disable();
}

void EditWindow::OnEnable()
{
	m_edit = 0;
	m_markChangeTimer.Run();
}

void EditWindow::OnDisable()
{
	if (m_backFocus != nullptr)
	{
		GetContext()->SetFocusUI(m_backFocus);
		GetContext()->Draw();
	}
}

bool EditWindow::OnEvent(char input)
{
	if (m_msgs.empty())
		return true;
	std::string &editStr = m_msgs[m_edit].resualt;
	switch (input)
	{
	case '<':
		Disable();
		break;
	case '\r':
		if (OnEditFinish(m_msgs))
			Disable();
		break;
	case '\b':
		if (editStr.size() > 0)
			if (editStr[editStr.size() - 1] > 0)
				editStr.erase(editStr.size() - 1, 1);
			else
				editStr.erase(editStr.size() - 2, 2);
		break;
	case '\t':
		DrawNormalData(m_edit);
		m_edit = (m_edit + 1) % m_msgs.size();
		break;
	default:
		if (m_msgs[m_edit].TestInput(input))
			m_msgs[m_edit].resualt += input;
		break;
	}
	return false;
}

void EditWindow::DrawEditData(int index)
{
	size_t showStrSize = m_view.GetCutRect().w - m_msgs[index].name.size() - 8;
	std::string data;
	if (m_msgs[index].isVisitable)
		data = m_msgs[index].name + " : " +
		m_msgs[index].resualt.substr((m_msgs[index].resualt.size() > showStrSize ? m_msgs[index].resualt.size() - showStrSize : 0), (m_msgs[index].resualt.size() > showStrSize ? showStrSize : m_msgs[index].resualt.size()))
		+ m_mark;
	else
		data = m_msgs[index].name + " : " + (m_msgs[index].resualt.size() > showStrSize ? std::string(showStrSize, '*') : std::string(m_msgs[index].resualt.size(), '*')) + m_mark;
	m_view.FillLine(1, 1, index * 3 + 2, ' ');
	m_view.SetText(2, index * 3 + 2, data);
	if (m_msgs[index].hasTips)
	{
		m_view.FillLine(1, 1, index * 3 + 3, ' ');
		m_view.SetText(2, index * 3 + 3, "#" + m_msgs[index].tips);
	}
	m_view.FillLine(1, 1, index * 3 + 4, ' ');
}

void EditWindow::DrawNormalData(int index)
{
	size_t showStrSize = m_view.GetCutRect().w - m_msgs[index].name.size() - 7;//" : "+frame
	std::string data;
	if (m_msgs[index].isVisitable)
		data = m_msgs[index].name + " : " +
		m_msgs[index].resualt.substr((m_msgs[index].resualt.size() > showStrSize ? m_msgs[index].resualt.size() - showStrSize : 0), (m_msgs[index].resualt.size() > showStrSize ? showStrSize : m_msgs[index].resualt.size()));
	else
		data = m_msgs[index].name + " : " + (m_msgs[index].resualt.size() > showStrSize ? std::string(showStrSize, '*') : std::string(m_msgs[index].resualt.size(), '*'));
	m_view.FillLine(1, 1, index * 3 + 2, ' ');
	m_view.SetText(2, index * 3 + 2, data);
	if (m_msgs[index].hasTips)
	{
		m_view.FillLine(1, 1, index * 3 + 3, ' ');
		m_view.SetText(2, index * 3 + 3, "#" + m_msgs[index].tips);
	}
	m_view.FillLine(1, 1, index * 3 + 4, ' ');
}
