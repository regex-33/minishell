/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 10:35:53 by bchanaa           #+#    #+#             */
/*   Updated: 2024/04/24 16:32:34 by bchanaa          ###   ########.fr       */
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

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_HERE,
	REDIR_OUT,
	REDIR_APPEND
}		t_redir_type;

typedef struct s_redir
{
	char			*filename;
	char			*delimiter;
	t_redir_type	type;
	int				fd;
}			t_redir;

typedef struct s_cmd
{
	t_list	*cmd_args;
	t_list	*redir_list;
}	t_cmd;

typedef struct s_info
{
	t_list	*processes;
}	t_info;

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
	nt_subcmd,
	nt_subredir,
	nt_simplecmd,
	nt_pipe,
	nt_and_if,
	nt_or_if,
	nt_io_redir,
	//nt_cmd_arg
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

int __exec(t_btree *tree, char **env); // i changed the prototype
int	panic(char *prog_name, int err, char c);


void			free_redir(void *redir);
t_redir			*new_redir(t_token *redir_token, t_token *file_token);
t_redir_type	getredir_type(char *str, int len);
int				getredir_fd(char *str, int len);

void	free_cmd(t_cmd *cmd);
t_cmd	*new_cmd(t_list *cmd_args, t_list *redir_list);

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
