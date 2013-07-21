# encoding: UTF-8

MCLIENT_BIN := $(BIN_DIR)/mclient

MCLIENT_INC_DIR := $(MCLIENT_DIR)/inc
MCLIENT_SRC_DIR := $(MCLIENT_DIR)/src
MCLIENT_OBJ_DIR := $(MCLIENT_DIR)/obj
MCLIENT_DEP_DIR := $(MCLIENT_DIR)/dep

# MCLIENT INCLUDES
MCLIENT_INCLUDES := -I $(MCLIENT_INC_DIR)
MCLIENT_INCLUDES += 

# MCLIENT DEFINITIONS
MCLIENT_DEFINITIONS := 
MCLIENT_DEFINITIONS += 

# MCLIENT SOURCES
MCLIENT_SRCS_NOTDIR := $(notdir $(wildcard $(MCLIENT_SRC_DIR)/*.c))

# MCLIENT OBJECTS
MCLIENT_OBJS := $(patsubst %.c, $(MCLIENT_OBJ_DIR)/%.o, $(MCLIENT_SRCS_NOTDIR))

# MCLIENT DEPENDENCIES
MCLIENT_DEPS := $(patsubst %.c, $(MCLIENT_DEP_DIR)/%.d, $(MCLIENT_SRCS_NOTDIR))

$(MCLIENT_OBJ_DIR)/%.o: $(MCLIENT_SRC_DIR)/%.c
	$(CC) -c $< $(FLAGS) $(MCLIENT_INCLUDES) -MMD -o $@
	@$(MV) $(patsubst %.o, %.d, $@) $(MCLIENT_DEP_DIR)

-include $(MCLIENT_DEPS)