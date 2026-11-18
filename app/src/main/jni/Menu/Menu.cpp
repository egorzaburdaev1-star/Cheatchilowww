#include "Includes/obfuscate.h"
#include "Menu/Menu.hpp"

void Init(JNIEnv *env, jobject thiz, jobject ctx, jobject title, jobject subtitle) {
    // Заголовок
    setText(env, title, OBFUSCATE("<b>Melstroynost96</b>"));

    // Подзаголовок (бегущая строка)
    setText(env, subtitle, OBFUSCATE("<b><marquee><p style=\"font-size:30\">"
                                     "<p style=\"color:green;\">Автор: Melstroynost96</p>"
                                     "</p></marquee></b>"));

    // Диалог при запуске
    Dialog(env,
           ctx,
           OBFUSCATE("Добро пожаловать в мод-меню"),
           OBFUSCATE("Автор: Melstroynost96"),
           nullptr,
           OBFUSCATE("Закрыть"),
           3,
           nullptr);

    // Всплывающее сообщение
    Toast(env, ctx, OBFUSCATE("Меню от Melstroynost96"), ToastLength::LENGTH_LONG);
}

jstring Icon(JNIEnv *env, jobject thiz) {
    return env->NewStringUTF(
            OBFUSCATE("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"));
}

jstring IconWebViewData(JNIEnv *env, jobject thiz) {
    // nullptr = использовать обычную картинку из Icon()
    return nullptr;
}

jobjectArray SettingsList(JNIEnv *env, jobject activityObject) {
    jobjectArray ret;

    const char *features[] = {
            OBFUSCATE("Category_Настройки"),
            OBFUSCATE("-1_Toggle_Сохранять настройки функций"),
            OBFUSCATE("-3_Toggle_Авторазмер по вертикали"),
            OBFUSCATE("Category_Меню"),
            OBFUSCATE("-6_Button_<font color='red'>Закрыть настройки</font>"),
    };

    int Total_Feature = (sizeof features / sizeof features[0]);
    ret = (jobjectArray)
            env->NewObjectArray(Total_Feature, env->FindClass(OBFUSCATE("java/lang/String")),
                                env->NewStringUTF(""));
    int i;
    for (i = 0; i < Total_Feature; i++)
        env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));

    return (ret);
}
