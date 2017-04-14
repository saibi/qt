#ifndef XNCLMESSAGEEXTRACTOR_H
#define XNCLMESSAGEEXTRACTOR_H

#include <QDomElement>
#include <QStringList>

class XnclMessageExtractor
{
public:
	XnclMessageExtractor();

	bool loadXML(const QString &filePath);
	bool loadXML(QIODevice *device);

	QStringList getMessageList(void);
	void setCheckTr(bool stat) { m_checkTr = stat; }
	const QString & getSectionString() { return m_section; }

protected:
	void parseGroupElement(const QDomElement &element, int depth);
	void parseItemElement(const QDomElement &element, int depth);
	void parseRadioElement(const QDomElement &element);
	void parseComboElement(const QDomElement &element);

	void addMessage(QString message);
	void saveMessageTable(QString file_path);

private:
	QDomDocument m_domDocument;

	QStringList m_message_list;

	bool m_checkTr;
	QString m_section;
};

#endif // XNCLMESSAGEEXTRACTOR_H
