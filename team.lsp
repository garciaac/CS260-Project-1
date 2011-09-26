;; returns cons cell whose car points to 'COOP or 'DEFECT and the 
;; cdr points to the history structure
(defun decide '(opID history last-op-decision opchange mychange)
)

;; history is opponent specific and returns c or d
(defun make-choice '(func ophistory)
)

;; returns a function (in its list form)
(defun look-up '(prob-data)
)

;; returns a real number which is the value of the probability
;; distribution evaluated at x
(defun prob '(prob-data mychoice opchoice x)
)
