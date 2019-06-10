#ifndef VIEWRECT_H
#define VIEWRECT_H

#include <memory>
#include <string>

struct Rect
{
	int x, y, w, h;
};

/*字符视图界面[一块矩形区域]*/
class ViewRect
{
public:
	ViewRect() = default;
	~ViewRect();

	/*以指定宽高构造一个ViewRect,并以ch填充*/
	void SetViewRect(int w, int h, char ch = ' ');
	/*引用另一个ViewRect*/
	void SetViewRect(const ViewRect &src);
	/*引用另一个ViewRect的某一块区域*/
	void SetViewRect(const ViewRect &src, const Rect &cut);
	/*移动ViewRect的源数据,获得原区域内存的释放权*/
	void SetViewRect(ViewRect &&src);

	/*通过view(x,y)即刻获取该位置对应字符的引用,下表从0开始*/
	inline char &operator()(int x, int y);
	inline const char &operator()(int x, int y)const;

	/*设置文本，若文本大小大于宽度，会显示省略号*/
	inline void SetText(int x, int y, const std::string &src);

	/*填充一行*/
	inline void FillLine(int xOffset, int y, char ch);
	inline void FillLine(int xOffset, int xLast, int y, char ch);

	/*获取剪切区域*/
	inline const Rect &GetCutRect()const;

	/*获取字符视图界面源数据*/
	inline char *GetViewData()const;

	/*重置剪裁区域*/
	inline void ResetCutView(char ch = ' ');
	/*将全部字符视图用ch重置*/
	inline void ResetSrcView(char ch = ' ');

	inline void Clear();

private:
	char *m_viewData = nullptr;
	bool m_isSrcData = true;
	Rect m_srcRect = {};
	Rect m_cutRect = {};
};

char &ViewRect::operator()(int x, int y)
{
	return m_viewData[(m_cutRect.y + y)*(m_srcRect.w + 1) + m_cutRect.x + x];
}

const char &ViewRect::operator()(int x, int y)const
{
	return m_viewData[(m_cutRect.y + y)*(m_srcRect.w + 1) + m_cutRect.x + x];
}

void ViewRect::SetText(int x, int y, const std::string &src)
{
	if (x >= m_cutRect.w || y >= m_cutRect.h)
		return;
	if (x + src.size() <= (size_t)m_cutRect.w)
		memcpy(&(*this)(x, y), src.data(), src.size());
	else
		memcpy(&(*this)(x, y), (src.substr(0, (m_cutRect.w - x - 3 > 0 ? m_cutRect.w - x - 3 : 0)) + "...").data(), m_cutRect.w);
}

inline void ViewRect::FillLine(int xOffset, int y, char ch)
{
	memset(&(*this)(xOffset, y), ch, m_cutRect.w - xOffset);
}

inline void ViewRect::FillLine(int xOffset, int xLast, int y, char ch)
{
	memset(&(*this)(xOffset, y), ch, m_cutRect.w - xOffset - xLast);
}

const Rect &ViewRect::GetCutRect() const
{
	return m_cutRect;
}

char *ViewRect::GetViewData() const
{
	return m_viewData;
}

void ViewRect::ResetCutView(char ch)
{
	for (int i = 0; i < m_cutRect.h; ++i)
		memset(&(*this)(0, i), ch, m_cutRect.w);
}

void ViewRect::ResetSrcView(char ch)
{
	const int &w = m_srcRect.w;
	const int &h = m_srcRect.h;
	memset(m_viewData, ch, sizeof(char)*(w + 1)*h);
	//填充换行及结束符
	for (int i = 0; i < h; ++i)
		m_viewData[i*(w + 1) + w] = '\n';
	m_viewData[h*(w + 1) - 1] = 0;
}

void ViewRect::Clear()
{
	if (m_viewData != nullptr&&m_isSrcData)
		delete m_viewData;
	m_viewData = nullptr;
	m_isSrcData = false;
}
#endif