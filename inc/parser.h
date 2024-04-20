/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 10:35:53 by bchanaa           #+#    #+#             */
/*   Updated: 2024/04/20 17:29:15 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# define TOKEN_D " \t|<>&()"

# define O_LETTERS 1
# define O_DIGITS 2

# define CONSUME_TOK 1
# define RESET_TOK 2

typedef enum e_token_type
{
		tok_l_par,
		tok_r_par,
		tok_literal,
		tok_pipe,
		tok_op_or,
		tok_op_and,
		tok_redir
}			t_token_type;

typedef struct s_token
{
		char			*value;
		size_t			len;
		t_token_type	type;
}			t_token;

typedef enum e_node_type
{
	nt_undefined,
	nt_cmd,
	nt_simplecmd,
	nt_pipe,
	nt_and_if,
	nt_or_if,
	nt_io_redir,
	nt_cmd_arg
} 	t_node_type;

// typedef struct s_tree
// {
// 	void			*value;
// 	struct s_tree	*sibling;
// 	struct s_tree	*child;
// 	t_node_type		type;
// }		t_tree;

typedef struct s_btree
{
	struct s_btree	*left;
	struct s_btree	*right;
	void			*data;
	t_node_type		type;
}			t_btree;

t_list	*lex_line(char *line);
void	print_token(t_token *token);
size_t	wordlen(char *str);
int		issep(char c);
void	print_token_list(t_list *token_list);
int		match_pattern(char *str, int flags, char *sentinel);
void	clear_tree(t_tree *root, void (*del)(void *));
t_tree	*new_sibling(t_tree *root, t_node_type type, void *val);
t_tree	*new_child(t_tree *parent, t_node_type type, void *val);
t_tree	*new_tree(t_node_type type, void *val, t_tree *sibling, t_tree *child);
void	print_tree(t_tree *root);
void	E(t_list *tokens);
void	P(t_list *tokens);


t_tree	*parse_cmd(t_list *token_list, int prec);
t_tree	*parse(t_list *token_list);
t_tree	*parse_simplecmd(t_list *token_list);
t_tree	*parse_pair(t_list *token_list, int prec);
