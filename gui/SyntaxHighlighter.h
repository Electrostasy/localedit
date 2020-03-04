#ifndef APB_CONFIG_SYNTAXHIGHLIGHTER_H
#define APB_CONFIG_SYNTAXHIGHLIGHTER_H

#include <QtGui/QSyntaxHighlighter>

class SyntaxHighlighter: public QSyntaxHighlighter {
    Q_OBJECT

public:
    explicit SyntaxHighlighter(QTextDocument *parent);

protected:
    void highlightBlock(const QString &text) override;

private:
    QRegularExpression statementExp;
    QTextCharFormat formatPunctuation;
    QTextCharFormat formatText;
    QTextCharFormat formatColour;
};


#endif
