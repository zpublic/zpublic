//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   KAVStrTranslate.h
//  Version     :   1.0
//  Creater     :   Freeway Chen
//  Date        :   2001-5-9 11:35:56
//  Comment     :   Kingsoft AntiVirus string translate header file
//
//////////////////////////////////////////////////////////////////////////////////////

#ifndef _KAVSTRTRANSLATE_H
#define _KAVSTRTRANSLATE_H


int inline _IntToStr(int nNum, WCHAR *pszStr, int nDigitalNum)
{
    while (nDigitalNum)
    {
        pszStr[nDigitalNum - 1] = TEXT('0') + (nNum % 10);
        
        nNum /= 10;
        
        nDigitalNum--;
    }
    
    return true;
}


int inline _IntToStr(int nNum, char *pszStr, int nDigitalNum)
{
    while (nDigitalNum)
    {
        pszStr[nDigitalNum - 1] = '0' + (nNum % 10);
        
        nNum /= 10;
        
        nDigitalNum--;
    }
    
    return true;
}

int inline _IntToStr(int nNum, int nStrSize, WCHAR *pszStr)
{
    nStrSize--; // use for last '\0' char

    nNum = nNum & 0x7fffffff;

    int i = 0;
    while (nStrSize > 0)
	{
        pszStr[i] = TEXT('0') + (short)(nNum % 10);

        nNum /= 10;

        nStrSize--;
        
        i++;

        if (nNum == 0)
            break;
	}
    pszStr[i] = '\0';

    int j = 0;
    while (j < (i / 2))
    {
        WCHAR ch = pszStr[j];
        pszStr[j] = pszStr[i - j - 1];
        pszStr[i - j - 1] = ch;

        j++;
    }


	return i;
}

int inline _IntToStr(int nNum, int nStrSize, char *pszStr)
{
    nStrSize--; // use for last '\0' char

    nNum = nNum & 0x7fffffff;

    int i = 0;
    while (nStrSize > 0)
	{
        pszStr[i] = '0' + (short)(nNum % 10);

        nNum /= 10;

        nStrSize--;
        
        i++;

        if (nNum == 0)
            break;
	}
    pszStr[i] = '\0';

    int j = 0;
    while (j < (i / 2))
    {
        char ch = pszStr[j];
        pszStr[j] = pszStr[i - j - 1];
        pszStr[i - j - 1] = ch;

        j++;
    }


	return i;
}

int inline _StrToInt(char *pszStr)
{
    int i;
    int nLen;
    int nNum = 0;

    nLen = strlen(pszStr);

    for (i = 0; i < nLen; i++)
    {
        if (
            (pszStr[i] >= '0') &&
            (pszStr[i] <= '9')
        )
        {
            nNum = nNum * 10 + (pszStr[i] - '0');
        }
        else
            break;
    }

    return nNum;
}

int inline _StrToInt(WCHAR *pszStr)
{
    int i;
    int nLen;
    int nNum = 0;

    nLen = wcslen(pszStr);

    for (i = 0; i < nLen; i++)
    {
        if (
            (pszStr[i] >= '0') &&
            (pszStr[i] <= '9')
        )
        {
            nNum = nNum * 10 + (pszStr[i] - '0');
        }
        else
            break;
    }

    return nNum;
}

unsigned inline _StrToUInt(char *pszStr)
{
    int i;
    int nLen;
    unsigned uNum = 0;

    nLen = strlen(pszStr);

    for (i = 0; i < nLen; i++)
    {
        if (
            (pszStr[i] >= '0') &&
            (pszStr[i] <= '9')
        )
        {
            uNum = uNum * 10 + (pszStr[i] - '0');
        }
        else
            break;
    }

    return uNum;
}

inline int _StrToInt(const char *nptr)
{
    int c;
    long total;
    int sign;

    while (true)
    {
        if (
            (*nptr != ' ') && 
            (*nptr != '\t')
        )
            break;

        nptr++;
    }

    c = (int)(unsigned char)*nptr++;
    sign = c;
    if (c == '-' || c == '+')
        c = (int)(unsigned char)*nptr++;

    total = 0;

    while ((c >= '0') && (c <= '9')) 
    {
        total = 10 * total + (c - '0');
        c = (int)(unsigned char)*nptr++;
    }

    if (sign == '-')
        return -total;
    else
        return total;   
}

int inline _HexToStrRight(int nNum, int nStrSize, TCHAR *pszStr)
{
    static TCHAR chHexToChar[] = 
    {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
    };

    nStrSize--;
    pszStr[nStrSize] = '\0';

    int nResidue = 0;
	while (nStrSize > 0)
	{
        nResidue = nNum % 0x10;
        
        pszStr[nStrSize - 1] = chHexToChar[nResidue];

        nNum /= 0x10;

        nStrSize--;
	}

	return true;
}

#endif // _KAVSTRTRANSLATE_H
