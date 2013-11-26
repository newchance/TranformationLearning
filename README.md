TranformationLearning
=====================
The program is the implementation of the paper "Transformation-Based Error-Driven Learning and Natural Language Processing: 
A Case Study in Part-Of-Speech Tagging". It's only the training part. The test program don't provide.

In this program, it's a simple rule-based approach to automated learning of linguistic knowledge.

The following is the architecture of program:

unannotated text
     |
initial state
     |
annotated text     truth
     |               |
      -   learner  -
             |
           rules


First, unannotated text is passed through an initial-state annotator.The initial-state annotator can range in complexity
assigning random structure to assigning the output of a sophisticated manually created annotator.

Once text has been passed through the initial-state annotator, it is then compared to the truth. An ordered list of 
transformations is learned that can be applied to the output of the initial-state annotator to make it better resemble 
the truth.
