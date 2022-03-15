#include "cino_string.h"

/**
 * @brief cino字符串结构
 */
typedef struct string_t {
    char *string;  // 字符串
    int length;    // 字符串长度
} string_t;

/****************************************
 *            cino字符串创建
 ****************************************/

/**
 * @brief   创建cino字符串
 * @param str   :   字符串
 * @return  返回构造好的cino字符串指针，失败返回NULL。
 */
string_t *string_create(const char *str) {
    return_value_if_fail(str != NULL, NULL);

    string_t *string = (string_t *)cino_alloc(sizeof(string_t));
    return_value_if_fail(string != NULL, NULL);

    string->length = str_length(str);

    string->string = (char *)calloc(string->length + 1, sizeof(char));
    call_and_return_value_if_fail(string->string != NULL, string_destroy(string), NULL);

    str_copy(string->string, str);
    return string;
}

/****************************************
 *            cino字符串销毁
 ****************************************/

/**
 * @brief   销毁cino字符串
 * @param string    :   cino字符串
 */
void string_destroy(string_t *string) {
    return_if_fail(string != NULL);

    string->length = 0;

    if (string->string) {
        free(string->string);
        string->string = NULL;
    }

    if (string) {
        free(string);
        string = NULL;
    }
}

/****************************************
 *            cino字符串操作
 ****************************************/

/**
 * @brief   获取cino字符串值
 * @param string    :   cino字符串
 * @return  cino字符串值
 */
const char *string_get(const string_t *string) {
    return_value_if_fail(string != NULL, NULL);
    return string->string;
}

/**
 * @brief   设置cino字符串
 * @param string    :   cino字符串
 * @param str       :   字符串值
 * @return  cino字符串
 */
string_t *string_set(string_t *string, const char *str) {
    return_value_if_fail(string != NULL, NULL);
    call_and_return_value_if_fail(str != NULL, string_clear(string), string);

    int str_len = str_length(str);
    if (string->length != str_len) {
        string->string = (char *)cino_realloc(string->string, sizeof(char) * (string->length + 1), sizeof(char) * (str_len + 1));
        call_and_return_value_if_fail(string->string != NULL, string_destroy(string), NULL);
    }

    str_copy(string->string, str);
    string->length = str_len;
    return string;
}

/**
 * @brief   获取cino字符串长度
 * @param string    :   cino字符串
 * @return  cino字符串长度
 */
int string_length(const string_t *string) {
    return_value_if_fail(string != NULL && string->string != NULL, 0);
    return string->length;
}

/**
 * @brief   清空cino字符串
 * @param string    :   cino字符串
 * @return  cino字符串
 */
string_t *string_clear(string_t *string) {
    return_value_if_fail(string != NULL, NULL);
    string->length = 0;
    string->string = (char *)cino_realloc(string->string, sizeof(char) * (string->length + 1), sizeof(char) * (string->length + 1));
    call_and_return_value_if_fail(string->string != NULL, string_destroy(string), NULL);
    str_clear(string->string, string->length + 1);
    return string;
}

/**
 * @brief   比较cino字符串
 * @param s1    :   cino字符串1
 * @param s2    :   cino字符串2
 * @return  如果s1和s2相同返回true，否则返回false。
 */
bool string_equal(const string_t *s1, const string_t *s2) {
    if (!s1 && !s2) {
        return true;
    }

    if (!s1 || !s2) {
        return false;
    }

    return str_equal(s1->string, s2->string);
}

/**
 * @brief   忽略大小写比较cino字符串
 * @param s1    :   cino字符串1
 * @param s2    :   cino字符串2
 * @return  如果s1和s2忽略大小写相同返回true，否则返回false。
 */
bool string_equal_ignore_case(const string_t *s1, const string_t *s2) {
    if (!s1 && !s2) {
        return true;
    }

    if (!s1 || !s2) {
        return false;
    }

    return str_equal_ignore_case(s1->string, s2->string);
}

/**
 * @brief   cino字符串转小写
 * @param string    :   cino字符串
 * @return  小写cino字符串
 */
string_t *string_to_lower(string_t *string) {
    return_value_if_fail(string != NULL, NULL);
    str_to_lower(string->string);
    return string;
}

/**
 * @brief   cino字符串转大写
 * @param string    :   cino字符串
 * @return  cino大写字符串
 */
string_t *string_to_upper(string_t *string) {
    return_value_if_fail(string != NULL, NULL);
    str_to_upper(string->string);
    return string;
}

/**
 * @brief   判断cino字符串是否以指定cino子串开头
 * @param string    :   cino字符串
 * @param prefix    :   cino子串
 * @return  如果string以prefix开头返回true，否则返回false。
 */
bool string_starts_with(string_t *string, string_t *prefix) {
    if (!string && !prefix) {
        return true;
    }
    return_value_if_fail(string != NULL && prefix != NULL, false);
    return str_starts_with(string->string, prefix->string);
}

/**
 * @brief   判断cino字符串是否以指定cino子串结尾
 * @param string    :   cino字符串
 * @param postfix    :   cino子串
 * @return  如果string以postfix结尾返回true，否则返回false。
 */
bool string_ends_with(string_t *string, string_t *postfix) {
    if (!string && !postfix) {
        return true;
    }
    return_value_if_fail(string != NULL && postfix != NULL, false);
    return str_ends_with(string->string, postfix->string);
}

/**
 * @brief   cino字符串拷贝
 * @param destination   :   目标cino字符串
 * @param source        :   源cino字符串
 * @return  返回目标cino字符串
 */
string_t *string_copy(string_t *destination, const string_t *source) {
    return_value_if_fail(destination != NULL, NULL);
    call_and_return_value_if_fail(source != NULL, string_clear(destination), destination);

    if (destination->length != source->length) {
        destination->string = (char *)cino_realloc(destination->string, sizeof(char) * (destination->length + 1), sizeof(char) * (source->length + 1));
        call_and_return_value_if_fail(destination->string != NULL, string_destroy(destination), NULL);
    }

    str_copy(destination->string, source->string);
    destination->length = source->length;
    return destination;
}

/**
 * @brief   cino字符串拼接
 * @param destination   :   目标cino字符串
 * @param source        :   源cino字符串
 * @return  返回目标cino字符串
 */
string_t *string_concat(string_t *destination, const string_t *source) {
    return_value_if_fail(destination != NULL && source != NULL, destination);
    destination->string = (char *)cino_realloc(destination->string, sizeof(char) * (destination->length + 1), sizeof(char) * (destination->length + source->length + 1));
    call_and_return_value_if_fail(destination->string != NULL, string_destroy(destination), NULL);
    str_concat(destination->string, source->string);
    destination->length += source->length;
    return destination;
}

/**
 * @brief   在cino字符串指定位置插入字符
 * @param string    :   cino字符串
 * @param pos       :   插入位置（从0开始）
 * @param c         :   字符
 * @return  新cino字符串
 */
string_t *string_insert_char(string_t *string, int pos, char c) {
    return_value_if_fail(string != NULL && pos >= 0 && pos <= string->length, string);

    int new_len = string->length + 1;
    string->string = (char *)cino_realloc(string->string, sizeof(char) * new_len, sizeof(char) * (new_len + 1));
    call_and_return_value_if_fail(string->string != NULL, string_destroy(string), NULL);

    str_insert_char(string->string, pos, c);

    // 如果插入'\0'会缩短字符串长度
    string->length = str_length(string->string);
    if (string->length < new_len) {
        string->string = (char *)cino_realloc(string->string, sizeof(char) * (new_len + 1), sizeof(char) * (string->length + 1));
        call_and_return_value_if_fail(string->string != NULL, string_destroy(string), NULL);
    }

    return string;
}

/**
 * @brief   在cino字符串指定位置插入cino子串
 * @param string    :   cino字符串
 * @param pos       :   插入位置（从0开始）
 * @param substr    :   cino子串
 * @return  新cino字符串
 */
string_t *string_insert_string(string_t *string, int pos, const string_t *substr) {
    return_value_if_fail(string != NULL && pos >= 0 && pos <= string_length(string) && substr != NULL, string);
    string->string = (char *)cino_realloc(string->string, sizeof(char) * (string->length + 1), sizeof(char) * (string->length + substr->length + 1));
    call_and_return_value_if_fail(string->string != NULL, string_destroy(string), NULL);
    str_insert_string(string->string, pos, substr->string);
    return string;
}