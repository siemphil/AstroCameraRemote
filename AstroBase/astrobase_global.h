#ifndef ASTROBASE_GLOBAL_H
#define ASTROBASE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ASTROBASE_LIBRARY)
#  define ASTROBASESHARED_EXPORT Q_DECL_EXPORT
#else
#  define ASTROBASESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // ASTROBASE_GLOBAL_H
