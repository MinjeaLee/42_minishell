#include "minishell.h"

static int change_question(t_info *info, t_cha_env *cv)
{
    if (info->ac < 2)
        return (FAILURE);
    cv->tmp1 = ft_strjoin(info->av[1], &cv->token->s[cv->string_index + 2]);
    if (cv->tmp1 == NULL)
        return (FAILURE);
    free(cv->token->s);
    cv->token->s = cv->tmp1;
    return (SUCCESS);
}

static int change_last_argument(t_cha_env *cv, t_info *info)
{
    cv->tmp1 = ft_strjoin(info->av[info->ac - 1], &cv->token->s[cv->string_index + 2]);
    if (cv->tmp1 == NULL)
        return (FAILURE);
    free(cv->token->s);
    cv->token->s = cv->tmp1;
    return (SUCCESS);
}

int  change_abs(t_info *info, t_cha_env *cv)
{
    cv->tmp1 = ft_strjoin(get_env_val("HOME", info), &cv->token->s[cv->string_index + 1]);
    if (cv->tmp1 == NULL)
        return (FAILURE);
    cv->tmp2 = ft_split(cv->tmp1, '=')[1];
    free(cv->token->s);
    free(cv->tmp1);
    cv->token->s = cv->tmp2;
    return (SUCCESS);
}

static int  change_child_exit_code(t_cha_env *cv)
{
    cv->exit_code = ft_itoa(g_exit_code);
    if (cv->exit_code == NULL)
        return (FAILURE);
    cv->token->s[cv->string_index] = '\0';
    cv->tmp1 = ft_strjoin(cv->token->s, cv->exit_code);
    if (cv->tmp1 == NULL)
    {
        free(cv->exit_code);
        return (FAILURE);
    }
    cv->tmp2 = ft_strjoin(cv->tmp1, &cv->token->s[cv->string_index + 2]);
    if (cv->tmp2 == NULL)
    {
        free(cv->exit_code);
        free(cv->tmp1);
        return (FAILURE);
    }
    free(cv->token->s);
    cv->token->s = cv->tmp2;
    cv->string_index += ft_strlen(cv->exit_code) - 1;
    free(cv->exit_code);
    free(cv->tmp1);
    return (SUCCESS);
}

int change_special_env(t_info *info, t_cha_env *cv)
{
    if (cv->token->s[cv->string_index + 1] == '?')
        return (change_child_exit_code(cv));
    else if (cv->token->s[cv->string_index + 1] == '_')
        return (change_last_argument(cv, info));
    else if (cv->token->s[cv->string_index + 1] == '-')
    {
        if (change_question(info, cv) == FAILURE)
            return (FAILURE);
    }
    return (SUCCESS);
}
