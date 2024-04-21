/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 10:35:53 by bchanaa           #+#    #+#             */
/*   Updated: 2024/04/21 11:49:26 by bchanaa          ###   ########.fr       */
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

t_list	*lexer(char *line);

int __exec(t_btree *tree);

void	clear_btree(t_btree *tree, void (*del)(void *));
void	print_tree(t_btree *tree, int depth, t_node_type parent_type);
t_btree	*new_node(t_node_type type, void *data, t_btree *left, t_btree *right);
t_btree	*new_leaf(t_node_type type, void *data);

t_btree	*parse_simplecmd(t_list *tokens);
t_btree	*parse_pair(t_list *tokens);
t_btree	*parse_cmd(t_list *tokens, int prec);
t_btree	*parse(t_list *tokens);

void	print_token(t_token *token);
size_t	wordlen(char *str);
int		issep(char c);
void	print_token_list(t_list *token_list);
int		match_pattern(char *str, int flags, char *sentinel);
void	E(t_list *tokens);
void	P(t_list *tokens);
