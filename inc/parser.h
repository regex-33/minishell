/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 10:35:53 by bchanaa           #+#    #+#             */
/*   Updated: 2024/04/21 18:29:53 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# define TOKEN_D " \t|<>&()"

# define O_LETTERS 1
# define O_DIGITS 2

# define CONSUME_TOK 1
# define RESET_TOK 2

# define PERR_UNEXP_TOK 1
# define PERR_UNC_PAR 2
# define PERR_UNC_QUOT 3
# define PERR_NEAR 4
# define PERR_EXP_TOK 5


// typedef struct	s_parsed_command
// {
// 	char		**command;
// 	char		*redirect_operator;
// 	char		*redirect_file;
// }				t_parsed_command;

typedef struct s_info
{
	t_list	*processes;
}	t_info;

typedef enum s_redir_ops
{
	REDIR_OUT,
	REDIR_OUT_APPEND,
	REDIR_IN,
	REDIR_HERE_DOC,
}			t_redir_ops;

struct s_redir
{
	t_redir_ops	type; // redirection type
	char		*filename;
	char		*delimiter; // for here_doc 
	int			fd; // file descriptor (this)>
}

typedef enum e_token_type
{
		tok_undefined,
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

int __exec(t_btree *tree, char **path_dirs);
int	panic(char *prog_name, int err, char c);

void	clear_btree(t_btree *tree, void (*del)(void *));
void	print_tree(t_btree *tree, int depth, t_node_type parent_type);
t_btree	*new_node(t_node_type type, void *data, t_btree *left, t_btree *right);
t_btree	*new_leaf(t_node_type type, void *data);

t_token	*next_token(t_list *token_list, int flags);

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

/* youssef function */


t_parsed_command	*parse_command(char *input);
char	*trim(char *str);
void	free_parsed_commands(t_parsed_command *parsed_commands);