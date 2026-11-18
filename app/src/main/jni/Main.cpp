#include <list>
#include <vector>
#include <cstring>
#include <pthread.h>
#include <thread>
#include <string>
#include <jni.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <dlfcn.h>
#include "Includes/Logger.h"
#include "Includes/obfuscate.h"
#include "Includes/Utils.hpp"
#include "Menu/Menu.hpp"
#include "Menu/Jni.hpp"
#include "Includes/Macros.h"
#include "dobby.h"

// ===================== ПЕРЕМЕННЫЕ (их меняют кнопки меню) =====================
bool godMode = false;     // бессмертие
int  fireRate = 1;        // скорострельность (1 = обычная)
bool flyMode = false;     // полёт
int  flySpeed = 5;        // скорость полёта
bool aimbot = false;      // автонаводка
bool espBox = false;      // ESP: рамка
bool espLine = false;     // ESP: линии
int  espR = 0, espG = 255, espB = 0; // ESP: цвет (красный, зелёный, синий)

// ===================== СПИСОК ФУНКЦИЙ В МЕНЮ =====================
// Номер перед названием (0_, 1_ ...) должен совпадать с case в Changes.
jobjectArray GetFeatureList(JNIEnv *env, jobject context) {
    jobjectArray ret;

    const char *features[] = {
            OBFUSCATE("Category_Основное"),
            OBFUSCATE("0_Toggle_Бессмертие"),
            OBFUSCATE("1_SeekBar_Скорострельность_1_10"),
            OBFUSCATE("2_Toggle_Полёт"),
            OBFUSCATE("3_SeekBar_Скорость полёта_1_20"),
            OBFUSCATE("4_Toggle_Автонаводка"),

            OBFUSCATE("Category_ESP"),
            OBFUSCATE("5_Toggle_Рамка"),
            OBFUSCATE("6_Toggle_Линии"),
            OBFUSCATE("7_SeekBar_Цвет: красный_0_255"),
            OBFUSCATE("8_SeekBar_Цвет: зелёный_0_255"),
            OBFUSCATE("9_SeekBar_Цвет: синий_0_255"),
    };

    int Total_Feature = (sizeof features / sizeof features[0]);
    ret = (jobjectArray)
            env->NewObjectArray(Total_Feature, env->FindClass(OBFUSCATE("java/lang/String")),
                                env->NewStringUTF(""));

    for (int i = 0; i < Total_Feature; i++)
        env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));

    return (ret);
}

//Target main lib here
#define targetLibName OBFUSCATE("libil2cpp.so")

// ===================== ОБРАБОТКА КНОПОК МЕНЮ =====================
void Changes(JNIEnv *env, jclass clazz, jobject obj, jint featNum, jstring featName, jint value, jlong Lvalue, jboolean boolean, jstring text) {
    switch (featNum) {
        case 0: godMode = boolean; break;
        case 1: fireRate = value; break;
        case 2: flyMode = boolean; break;
        case 3: flySpeed = value; break;
        case 4: aimbot = boolean; break;
        case 5: espBox = boolean; break;
        case 6: espLine = boolean; break;
        case 7: espR = value; break;
        case 8: espG = value; break;
        case 9: espB = value; break;
        default: break;
    }
}

// ===================== ХУКИ =====================
// Сигнатуры ниже ПРИМЕРНЫЕ. Перед включением сверьте их с dump.cs:
// тип возвращаемого значения и аргументы должны совпадать с методом игры.

// --- Бессмертие: метод получения урона игроком ---
void (*old_TakeDamage)(void *instance, float damage);
void TakeDamage(void *instance, float damage) {
    if (instance != nullptr && godMode) {
        // TODO: добавить проверку, что instance это ваш игрок, а не бот
        return;
    }
    old_TakeDamage(instance, damage);
}

// --- Скорострельность: геттер задержки между выстрелами ---
float (*old_GetFireDelay)(void *instance);
float GetFireDelay(void *instance) {
    float orig = old_GetFireDelay(instance);
    if (instance != nullptr && fireRate > 1) {
        return orig / (float) fireRate;
    }
    return orig;
}

// --- Полёт, автонаводка, ESP: хук на Update игрока ---
void (*old_PlayerUpdate)(void *instance);
void PlayerUpdate(void *instance) {
    if (instance != nullptr) {
        if (flyMode) {
            // TODO: поднимать игрока вверх (set_position / CharacterController.Move)
        }
        if (aimbot) {
            // TODO: найти ближайшего врага и повернуть камеру
        }
        if (espBox || espLine) {
            // TODO: передать координаты врагов в оверлей, цвет (espR, espG, espB)
        }
    }
    old_PlayerUpdate(instance);
}

// ===================== ЗАПУСК =====================
void hack_thread() {
    // libil2cpp.so грузится не сразу, ждём
    while (!isLibraryLoaded(targetLibName)) {
        sleep(1);
    }

#if defined(__aarch64__)
    // Хуки ВЫКЛЮЧЕНЫ, пока нет реальных смещений из dump.cs.
    // Когда найдёте метод, замените 0x0 на его RVA и уберите //

    // HOOK(targetLibName, "0x0", TakeDamage, old_TakeDamage);
    // HOOK(targetLibName, "0x0", GetFireDelay, old_GetFireDelay);
    // HOOK(targetLibName, "0x0", PlayerUpdate, old_PlayerUpdate);

    LOGI(OBFUSCATE("Hooks ready"));
#elif defined(__arm__)
    // только для armv7
#endif

    LOGI(OBFUSCATE("Done"));
}

__attribute__((constructor))
void lib_main() {
    std::thread(hack_thread).detach();
}
